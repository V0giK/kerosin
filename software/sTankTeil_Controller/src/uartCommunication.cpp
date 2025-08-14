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
#include "defines.h"
#include "helper.h"

// Konstruktor
UartCommunication::UartCommunication(uint8_t rxPin, uint8_t txPin, bool debug)
    : softSerial(rxPin, txPin), debugEnabled(debug), currentBaudRate(0), writeCallback(nullptr), readCallback(nullptr) {}

// UART starten
void UartCommunication::begin(long baudRate) {
    currentBaudRate = baudRate;
    softSerial.begin(baudRate);
    debugPrint("UART-Kommunikation gestartet mit Baudrate: " + String(baudRate));
}

// Hauptschleife
bool UartCommunication::tick() {

    if (softSerial.available()) {

         String sVal = softSerial.readStringUntil('\n');
         sVal.trim();
         debugPrint("Empfangen: " + sVal + "-end");

        // ACK prüfen
        if (waitingForAck && sVal == "ACK") {
            //debugPrint("Empfangsbestätigung (ACK) erhalten!");
            waitingForAck = false;
            return true;
        }

         
        //if (sVal.startsWith("S")) {
            processReceivedData(sVal);
        //}
    }

    return false;

}

// Callback für SW setzen
void UartCommunication::setWriteCallback(void (*callback)(int16_t id, const String &value)) {
    writeCallback = callback;
}

// Callback für SR setzen
void UartCommunication::setReadCallback(void (*callback)(int16_t id)) {
    readCallback = callback;
}

// Daten senden mit Retry-Mechanismus
bool UartCommunication::sendData(char rw, int16_t id, const char *data, bool waitForAck, int maxRetries) {

    if(currentBaudRate == 0) return false;

    if (rw != 'R') rw = 'W';
    setacki(id, waitForAck);

    char message[128]; // Puffer
    snprintf(message, sizeof(message), "S%c:%d:%s", rw, id, data);
    uint16_t crc = calculateCRC(message);
    char packet[128]; // Puffer
    snprintf(packet, sizeof(packet), "%s:%u", message, crc);

    int retries = 0;
    while (retries <= maxRetries) {
        softSerial.println(packet);
        debugPrint("Gesendet: " + String(packet) + " (Versuch " + String(retries + 1) + ")");

        if (waitForAck) {
            waitingForAck = true;
            ackStartTime = millis();

            // Nicht-blockierendes Warten auf ACK
            while (millis() - ackStartTime < ackTimeout) {
                if (!waitingForAck) {
                    //debugPrint("ACK erfolgreich erhalten.");
                    return true; // Erfolg
                }
                tick(); // Eingehende Nachrichten prüfen
            }

            //debugPrint("Timeout: ACK nicht erhalten.");
            waitingForAck = false;
        } else {
            return true; // Erfolg, wenn kein ACK benötigt wird
        }

        retries++;
    }

    //debugPrint("Fehler: Max. Wiederholungen erreicht. Keine ACK erhalten.");
    return false; // Fehler nach allen Wiederholungen
}

// Empfangene Daten verarbeiten
void UartCommunication::processReceivedData(const String &data) {

    if (!data.startsWith("S")) {
        //debugPrint("NACK Fehler: Ungültiges Format! - Beginnt nicht mit 'S'");
        softSerial.println("NACK");
        return;
    }

    int firstColon = data.indexOf(':');
    int secondColon = data.indexOf(':', firstColon + 1);
    int thirdColon = data.lastIndexOf(':');

    if (firstColon == -1 || secondColon == -1 || thirdColon == -1) {
        //debugPrint("NACK Fehler: Ungültiges Format!");
        softSerial.println("NACK");
        return;
    }

    String action = data.substring(0, firstColon);
    int16_t id = data.substring(firstColon + 1, secondColon).toInt();
    String value = data.substring(secondColon + 1, thirdColon);
    uint16_t receivedCRC = data.substring(thirdColon + 1).toInt();

    // CRC prüfen
    String checkData = action + ":" + id + ":" + value;
    uint16_t calculatedCRC = calculateCRC(checkData.c_str());
    if (calculatedCRC != receivedCRC) {
        if(isacki(id)) {
            //debugPrint("NACK Fehler: CRC ungültig! ");
            softSerial.println("NACK");
        }
        return;
    }

    // ACK senden
    if(isacki(id)) {
        softSerial.println("ACK");
        //debugPrint("ACK gesendet");
    }
    clracki(id);

    if (action == "SW" && writeCallback) {
        writeCallback(id, value); // Callback für "SW" aufrufen
    } else if (action == "SR" && readCallback) {
        readCallback(id); // Callback für "SR" aufrufen
    }

    //debugPrint("Verarbeitet: Aktion=" + action + ", ID=" + String(id) + ", Wert=" + value);
}

// Debug-Ausgabe
void UartCommunication::debugPrint(const String &message) {
    if (debugEnabled) {
        Serial.println("[DEBUG] " + message);
        delay(10);
    }
}

// SoftwareSerial neu starten
void UartCommunication::resetSerial() {
    if(currentBaudRate == 0) return;

    //debugPrint("SoftwareSerial wird neu gestartet...");
    softSerial.end();
    delay(500);
    softSerial.begin(currentBaudRate);
    //debugPrint("SoftwareSerial neu gestartet.");
}


