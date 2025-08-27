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

#include <Arduino.h>
#include <CircularBuffer.hpp>

class UartCommunication {
public:
    UartCommunication(int rxPin, int txPin, bool debug = false, int uartNumber = 1);
    
    void begin(long baudRate = 9600);
    void reset(); // UART Schnittstelle zurücksetzen
    void tick(); // Empfangen von Daten verarbeiten
    bool sendData(char rw, int16_t id, const String &data = "", bool waitForAck = false, int maxRetries = 3);
    bool getRequestResponse(int16_t id, String &response);
    bool processHasHardwareType(const String &data);
    bool isConnected() const { return connected && (millis() - lastMessageTime < CONNECTION_TIMEOUT); }

private:
    // Konstanten
    static const uint32_t CONNECTION_TIMEOUT = 3000;  // 3s
    static const uint32_t ACK_TIMEOUT = 2000;        // 2s
    static const size_t RX_BUFFER_SIZE = 256;
    static const size_t MSG_QUEUE_SIZE = 8;

    // Hardware
    HardwareSerial uart;
    int rxPin, txPin;
    bool debugEnabled;
    int currentBaudRate;

    // Status
    bool connected = false;
    uint32_t lastMessageTime = 0;
    bool waitingForAck = false;
    bool isNACK = false;
    uint32_t ackStartTime = 0;

    // Buffer Management
    char rxBuffer[RX_BUFFER_SIZE];
    size_t rxIndex = 0;
    String lastReceivedResponse;
    int16_t lastResponseID;
       struct Message {
        String data;
        uint32_t timestamp;
        int16_t id;
        bool needsAck;
    };
    CircularBuffer<Message, MSG_QUEUE_SIZE> messageQueue;

    // Hilfsfunktionen
    uint16_t calculateCRC(const char *data);
    void processReceivedData(const String &data);
    void debugPrint(const String &message);
    void handleTimeout();
    void enterDeepSleepForeverESP32();
    void processIncomingData();
    void processMessageQueue();
};

#endif
