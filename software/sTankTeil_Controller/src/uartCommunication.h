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

#ifndef UARTCOMMUNICATION_H
#define UARTCOMMUNICATION_H

//#include <SoftwareSerial.h>
#include <NeoSWSerial.h>
#include <Arduino.h>

class UartCommunication {
public:
    // Konstruktor
    UartCommunication(uint8_t rxPin, uint8_t txPin, bool debug = false);

    void begin(long baudRate);
    bool tick();

    // Daten senden mit optionaler Empfangsbestätigung und Retry-Mechanismus
    bool sendData(char rw, int16_t id, const char *data, bool waitForAck = false, int maxRetries = 3);

    // Callbacks für "SW" und "SR" setzen
    void setWriteCallback(void (*callback)(int16_t id, const String &value));
    void setReadCallback(void (*callback)(int16_t id));

    // Neustarten der SoftwareSerial-Verbindung
    void resetSerial();

private:
    void processReceivedData(const String &data);
    void debugPrint(const String &message);

    NeoSWSerial softSerial;

    bool debugEnabled;

    // ACK-Warte-Variablen
    bool waitingForAck = false;
    unsigned long ackStartTime = 0;
    const unsigned long ackTimeout = 2000; // Timeout in ms

    long currentBaudRate = 0;

    // Funktionszeiger für Callbacks
    void (*writeCallback)(int16_t id, const String &value);
    void (*readCallback)(int16_t id);
};

#endif
