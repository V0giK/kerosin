/*
 * Hauptprogramm zur Steuerung einer Arduino-basierten Tanklösung
 *
 * Copyright (C) 2025 V0giK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "UartCommunication.h"
#include "helper.h"
#include "ui/vars.h"
#include "main.h"

// Konstruktor
UartCommunication::UartCommunication(int rxPin, int txPin, bool debug, int uartNumber)
    : rxPin(rxPin), txPin(txPin), debugEnabled(debug), uart(uartNumber), 
      currentBaudRate(0), rxIndex(0) {}

// UART initialisieren
void UartCommunication::begin(long baudRate) {
    currentBaudRate = baudRate;
    uart.begin(baudRate, SERIAL_8N1, rxPin, txPin);
    // uart.setTimeout(10000);  // Timeout auf 10000 ms einstellen
    delay(100);
    debugPrint("UART started at " + String(baudRate) + " baud");
}

// UART Schnittstelle zurücksetzen
void UartCommunication::reset() {
    if(currentBaudRate == 0) return;

    uart.end();
    delay(100);
    uart.begin(currentBaudRate, SERIAL_8N1, rxPin, txPin); // Standard-Baudrate wiederherstellen
    delay(100);
    
    // Reset status
    rxIndex = 0;
    waitingForAck = false;
    isNACK = false;
    lastResponseID = -1;
    
    debugPrint("UART reset completed");
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

    char message[RX_BUFFER_SIZE]; // Puffer
    snprintf(message, sizeof(message), "S%c:%d:%s", rw, id, data.c_str());
    uint16_t crc = calculateCRC(message);

    char packet[RX_BUFFER_SIZE]; // Puffer
    snprintf(packet, sizeof(packet), "%s:%u\n", message, crc);

    int retries = 0;
    while (retries <= maxRetries) {
        uart.print(packet);
        debugPrint("Gesendet: " + String(packet) + " (Versuch " + String(retries + 1) + ")");

        if (waitForAck) {
            waitingForAck = true;
            isNACK = false;
            ackStartTime = millis();

            // Warte auf ACK mit Timeout
            while (millis() - ackStartTime < ACK_TIMEOUT) {
                tick(); // Eingehende Nachrichten prüfen
                if (!waitingForAck) {
                    debugPrint("ACK erfolgreich erhalten.");
                    return true; // Erfolg
                }
                if(isNACK) {
                    debugPrint("NACK erfolgreich erhalten.");
                    break;
                }
                yield();
            }

            debugPrint("Timeout oder NACK: ACK nicht erhalten.");
        } else {
            return true; // Erfolg, wenn kein ACK benötigt wird
        }

        retries++;
        if (retries <= maxRetries) {
            delay(50 * retries);  // Exponential backoff
        }
    }

    debugPrint("Send failed after " + String(maxRetries) + " retries");
    return false; // Fehler nach allen Wiederholungen
}

// Empfangen von Daten verarbeiten
void UartCommunication::tick() {
    if(currentBaudRate == 0) return;
    
    // Process incoming messages
    processIncomingData();
    
    // Process queued messages
    processMessageQueue();

}

// Verarbeite eingehende Daten
void UartCommunication::processIncomingData() {
    while (uart.available()) {
        char c = uart.read();
        lastMessageTime = millis();
        connected = true;

        if (c == '\n') {
            rxBuffer[rxIndex] = '\0';
            String message = String(rxBuffer);
            message.trim();
            
            if (message.length() > 0) {
                Message msg = {
                    .data = message,
                    .timestamp = millis(),
                    .id = -1,  // Will be parsed later
                    .needsAck = false
                };
                
                if (!messageQueue.isFull()) {
                    messageQueue.push(msg);
                    debugPrint("Message queued: " + message);
                } else {
                    debugPrint("Message queue full, dropping: " + message);
                }
            }
            rxIndex = 0;
        } else if (rxIndex < RX_BUFFER_SIZE - 1) {
            rxBuffer[rxIndex++] = c;
        }
    }
}

// Verarbeite die Nachrichtenwarteschlange
void UartCommunication::processMessageQueue() {
    while (!messageQueue.isEmpty()) {
        Message msg = messageQueue.first();
        
        // Handle ACK/NACK messages immediately
        if (msg.data == "ACK" && waitingForAck) {
            waitingForAck = false;
            messageQueue.shift();
            debugPrint("ACK processed from queue");
            continue;
        }
        
        if (msg.data == "NACK" && waitingForAck) {
            isNACK = true;
            messageQueue.shift();
            debugPrint("NACK processed from queue");
            continue;
        }

        // Process normal messages
        if (msg.data.startsWith("S")) {
            processReceivedData(msg.data);
        }
        
        messageQueue.shift();
    }

    // Check for timeouts
    if (waitingForAck && (millis() - ackStartTime > ACK_TIMEOUT)) {
        handleTimeout();
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

// Timeout-Handling
void UartCommunication::handleTimeout() {
    debugPrint("Timeout occurred - resetting connection state");
    
    // Reset connection flags
    waitingForAck = false;
    isNACK = false;
    connected = false;
    
    // Clear buffers
    rxIndex = 0;
    while (!messageQueue.isEmpty()) {
        messageQueue.shift();
    }
    while (uart.available()) {
        uart.read();
    }
    
    // Reset response tracking
    lastResponseID = -1;
    lastReceivedResponse = "";
    
    // Optional: Notify UI about connection loss
    set_var_s_status("Connection timeout");
    
    // Optional: Reset pump if active
    if (get_var_b_is_pumping()) {
        set_var_b_is_pumping(false);
        set_var_b_disable_btn_home(false);
        set_var_b_disable_btn_settings(false);
    }
    
    debugPrint("Connection state reset completed");
}