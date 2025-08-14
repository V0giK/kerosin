#include "UartCommunication.h"
#include "helper.h"
#include "ui/vars.h"
#include "main.h"

// Konstruktor
UartCommunication::UartCommunication(int rxPin, int txPin, bool debug, int uartNumber)
    : rxPin(rxPin), txPin(txPin), debugEnabled(debug), uart(uartNumber), currentBaudRate(9600), waitingForAck(false), ackStartTime(0), isNACK(false), lastResponseID(-1) {}

// UART initialisieren
void UartCommunication::begin(long baudRate) {
    currentBaudRate = baudRate;
    uart.begin(baudRate, SERIAL_8N1, rxPin, txPin);
    // uart.setTimeout(10000);  // Timeout auf 10000 ms einstellen
    delay(100);
    debugPrint("UART gestartet.");
}

// UART Schnittstelle zurücksetzen
void UartCommunication::reset() {
    if(currentBaudRate == 0) return;

    uart.end();
    delay(100);
    uart.begin(currentBaudRate, SERIAL_8N1, rxPin, txPin); // Standard-Baudrate wiederherstellen
    // uart.setTimeout(10000);  // Timeout auf 10000 ms einstellen
    delay(100);
    debugPrint("UART Schnittstelle wurde zurückgesetzt.");
}

// CRC-16 Berechnung
uint16_t UartCommunication::calculateCRC(const char *data) {
    uint16_t crc = 0xFFFF; // Initialer CRC-Wert

    while (*data) { // Iteriere über den C-String
        crc ^= static_cast<uint16_t>(*data++) << 8; // Byteweise Verarbeitung
        
        for (uint8_t j = 0; j < 8; j++) {
            // Falls höchstes Bit gesetzt ist, XOR mit dem Generatorpolynom
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021; // CRC-CCITT Standard
            } else {
                crc <<= 1;
            }
        }
    }

    return crc & 0xFFFF; // Sicherstellen, dass Ergebnis 16-Bit bleibt
}

// Daten senden mit Retry-Mechanismus
bool UartCommunication::sendData(char rw, int16_t id, const String &data, bool waitForAck, int maxRetries) {
    if(currentBaudRate == 0) return false;
    
    if (rw != 'R') rw = 'W';
    setacki(id, waitForAck); // ACK Bit setzen

    //String message = "S" + String(rw) + ":" + String(id) + ":" + data;
    char message[130]; // Puffer
    // snprintf(message, sizeof(message), "S%c:%d:%s", rw, id, data);
    snprintf(message, sizeof(message), "S%c:%d:%s", rw, id, data);
    uint16_t crc = calculateCRC(message);
    //String packet = message + ":" + String(crc);
    char packet[130]; // Puffer
    snprintf(packet, sizeof(packet), "%s:%u", message, crc);


    int retries = 0;
    while (retries <= maxRetries) {
        uart.println(packet);
        debugPrint("Gesendet: " + String(packet) + " (Versuch " + String(retries + 1) + ")");

        if (waitForAck) {
            waitingForAck = true;
            ackStartTime = millis();

            // Warte auf ACK mit Timeout
            while (millis() - ackStartTime < ackTimeout) {
                if (!waitingForAck) {
                    debugPrint("ACK erfolgreich erhalten.");
                    return true; // Erfolg
                }
                if(isNACK) {
                    debugPrint("NACK erfolgreich erhalten.");
                    break;
                }
                tick(); // Eingehende Nachrichten prüfen
            }

            debugPrint("Timeout oder NACK: ACK nicht erhalten.");
            isNACK = false;
            waitingForAck = false;
        } else {
            return true; // Erfolg, wenn kein ACK benötigt wird
        }

        retries++;
    }

    debugPrint("Fehler: Max. Wiederholungen erreicht. Keine ACK erhalten.");
    return false; // Fehler nach allen Wiederholungen
}

// Empfangen von Daten verarbeiten
void UartCommunication::tick() {
    if(currentBaudRate == 0) return;
    if (uart.available()) {
        String sVal = uart.readStringUntil('\n');
        sVal.trim();
        debugPrint("Empfangen: " + sVal + "-end");

        // ACK prüfen
        if (waitingForAck && sVal == "ACK") {
            debugPrint("Empfangsbestätigung (ACK) erhalten!");
            waitingForAck = false;
            return;
        }
        // NACK prüfen
        if (waitingForAck && !isNACK && sVal == "NACK") {
            debugPrint("NACK erhalten!");
            isNACK = true;
            return;
        }
        
        if (sVal.startsWith("S")) {
            processReceivedData(sVal);
        }
    }
}

// Antwort abrufen
bool UartCommunication::getRequestResponse(int16_t id, String &response) {
    if (lastResponseID == id) {
        response = lastReceivedResponse;
        lastResponseID = -1; // Antwort wurde verarbeitet
        return true;
    }
    return false; // Keine Antwort für diese ID
}

// Empfangene Daten verarbeiten
void UartCommunication::processReceivedData(const String &data) {
    int firstColon = data.indexOf(':');
    int secondColon = data.indexOf(':', firstColon + 1);
    int thirdColon = data.lastIndexOf(':');

    if (firstColon == -1 || secondColon == -1 || thirdColon == -1) {
        debugPrint("Fehler: Ungültiges Datenformat!");
        return;
    }

    String action = data.substring(0, firstColon);
    int16_t id = data.substring(firstColon + 1, secondColon).toInt();
    String value = data.substring(secondColon + 1, thirdColon);
    uint16_t receivedCRC = data.substring(thirdColon + 1).toInt();

    // CRC prüfen
    String checkData = action + ":" + id + ":" + value;
    // char checkData[128]; // Puffer
    // snprintf(checkData, sizeof(checkData), "%s:%d:%s", action, id, value);
    uint16_t calculatedCRC = calculateCRC(checkData.c_str());
    if (calculatedCRC != receivedCRC) {
        debugPrint("Fehler: CRC ungültig!");
        return;
    }

    // ACK senden
    if(isacki(id)) {
        uart.println("ACK");
        debugPrint("ACK gesendet");
    }
    clracki(id); // ACK Bit entfernen

    // Speichere die Antwort
    lastReceivedResponse = value;
    lastResponseID = id;

    // Daten verarbeiten
    if (action == "SW") {
        // if (id == COM_ID_TYPE) {
        //     // Gerätetyp: Noch keine Aktionen
        // } else if (id == COM_ID_FLOW_TICKS) {
        //     set_var_i_tick_per_liter(value.c_str());
        // } else if (id == COM_ID_PRESSURE_AVG) {
        //     set_var_i_pressure_count(value.c_str());
        // } else if (id == COM_ID_AKKU_MIN) {
        //     set_var_f_min_akku(value.c_str());
        // } else if (id == COM_ID_BEEP) {
        //     set_var_b_signal((value.toInt() == 1));
        // } else if (id == COM_ID_STATUS) {
        //     set_var_s_status(value.c_str());
        // } else if (id == COM_ID_CURRENT_ML) {
        //     set_var_i_fuel_ml(value.toInt());
        // }

        switch(id) {
            case COM_ID_CURRENT_ML:
                set_var_i_fuel_ml(value.toInt());
                set_var_s_fuel_ml(int2char(value.toInt(), LBL_POSTFIX_ML));
                set_var_s_cut_off_ml(int2char(atoi(get_var_s_max_refuel_ml()) - value.toInt(), LBL_POSTFIX_ML));
                break;
            case COM_ID_CURRENT_MBAR:
                set_var_s_fuel_mbar(int2char(value.toInt(), LBL_POSTFIX_MBAR));
                break;
            case COM_ID_CURRENT_DIFF_MBAR:
                set_var_s_fuel_diff_mbar(int2char(value.toInt(), LBL_POSTFIX_MBAR));
                break;
            case COM_ID_STATUS:
                set_var_s_status(value.c_str());
                break;
            case COM_ID_FLOWCALIBRATE:
                set_var_i_calib_flow_sensor(value.c_str());
                break;
            case COM_ID_PUMP_CONTROL:
                if(value.toInt() == CTR_STOP) {
                    // g_pumpStop
                    set_var_b_is_pumping(false);

                    // TODO: abhängig ob in Calib oder nicht
                    set_var_b_disable_btn_home(false);
                    set_var_b_disable_btn_settings(false);
                }
                break;
            case COM_ID_AKKU_VOLT:
                set_var_s_akku_volt((value + " V").c_str());
                break;

            case COM_ID_MC_RESET:
                if(value.equals("99")) { 
                    tft.setBrightness(0);
                    enterDeepSleepForeverESP32();
                } else {
                    esp_restart();
                }
                break;
        }

    } else if (action == "SR") {
        // SR (Read-Anfragen): TODO
    }
}

// Hardware-Typ verarbeiten
bool UartCommunication::processHasHardwareType(const String &data) {
    int firstColon = data.indexOf(':');
    int secondColon = data.indexOf(':', firstColon + 1);
    int thirdColon = data.lastIndexOf(':');

    if (firstColon == -1 || secondColon == -1 || thirdColon == -1) {
        debugPrint("Fehler: Ungültiges Format!");
        return false;
    }

    String action = data.substring(0, firstColon);
    int id = data.substring(firstColon + 1, secondColon).toInt();
    String value = data.substring(secondColon + 1, thirdColon);
    uint16_t receivedCRC = data.substring(thirdColon + 1).toInt();

    String checkData = action + ":" + id + ":" + value;
    uint16_t calculatedCRC = calculateCRC(checkData.c_str());
    if (calculatedCRC != receivedCRC) {
        debugPrint("Fehler: CRC ungültig!");
        return false;
    }

    clracki(id); // ACK Bit entfernen

    return (action == "SW" && id == COM_ID_TYPE);
}

// Debug-Ausgabe
void UartCommunication::debugPrint(const String &message) {
    if (debugEnabled) {
        Serial.println("[DEBUG] " + message);
    }
}

// Funktion, um den ESP32 in den Deep-Sleep-Modus zu versetzen
void UartCommunication::enterDeepSleepForeverESP32() {
    // Keine Wake-Up Quellen aktivieren, damit er nur durch Reset aufwacht
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);

    // Deep Sleep starten (er wacht nur durch einen Reset auf)
    esp_deep_sleep_start();
}