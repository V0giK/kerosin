// helper.cpp

#include <avr/sleep.h>
#include "helper.h"

// CRC-16 Berechnung
uint16_t calculateCRC(const char *data) {
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

// Arduino Nano in den Power-Down-Modus zu versetzen
void enterSleepNano() {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Power-Down-Modus
    cli(); // Interrupts deaktivieren, um sicherzustellen, dass nichts ihn aufweckt
    sleep_enable();

    // Brown-Out Detection deaktivieren (optional, spart noch mehr Strom)
    sleep_bod_disable();

    sleep_cpu(); // In den Schlafmodus gehen

    // Nach einem Power-Cycle würde der Arduino hier neu starten
}