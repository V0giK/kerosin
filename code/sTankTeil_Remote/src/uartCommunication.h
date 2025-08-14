#ifndef UARTCOMMUNICATION_H
#define UARTCOMMUNICATION_H

#include <Arduino.h>

class UartCommunication {
public:
    UartCommunication(int rxPin, int txPin, bool debug = false, int uartNumber = 1);

    void begin(long baudRate = 9600);
    void reset(); // UART Schnittstelle zurücksetzen
    void tick(); // Empfangen von Daten verarbeiten
    bool sendData(char rw, int16_t id, const String &data = "", bool waitForAck = false, int maxRetries = 3);
    bool getRequestResponse(int16_t id, String &response); // Antwort abrufen
    bool processHasHardwareType(const String &data);

private:
    HardwareSerial uart;
    int rxPin, txPin;
    bool debugEnabled;
    int currentBaudRate = 0;

    // ACK-Warte-Variablen
    bool waitingForAck = false;
    bool isNACK = false;
    unsigned long ackStartTime = 0;
    const unsigned long ackTimeout = 2000; // Timeout in ms

    String lastReceivedResponse; // Letzte empfangene Antwort
    int16_t lastResponseID;          // ID der letzten Antwort

    uint16_t calculateCRC(const char *data);
    void processReceivedData(const String &data);
    void debugPrint(const String &message);
    void enterDeepSleepForeverESP32();
};

#endif // UARTCOMMUNICATION_H
