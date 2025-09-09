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
#include "SystemConfigManager.h"
#include "variables.h"

// Konstruktor
SystemConfigManager::SystemConfigManager(int eepromAddress)
    : address(eepromAddress) {}

// Konfiguration aus EEPROM laden
void SystemConfigManager::loadConfig() {
    readConfigFromEEPROM();

    if (!isValid()) {
         #ifdef DEBUG
         Serial.println("Ungültige Konfiguration. Setze auf Standardwerte...");
         Serial.flush();
         #endif
         resetToDefaults();
     } else {
         #ifdef DEBUG
         Serial.println("Konfiguration erfolgreich geladen.");
         Serial.flush();
         #endif
     }

    // Serial.print("Manuelle Pumpenleistung:     [%] "); Serial.println(config.manuellePumpenleistung); Serial.flush();
    // Serial.print("Impulse pro Liter:               "); Serial.println(config.impulseProLiter); Serial.flush();
    // Serial.print("Minimale Akkuspannung:      [mV] "); Serial.println(config.minimalspannungAkku); Serial.flush();
    // Serial.print("Anzahl Messwerte Mittelwert:     "); Serial.println(config.anzahlMesswerteMittelwert); Serial.flush();
    // Serial.print("Systemabschaltung:        [Sek.] "); Serial.println(config.systemabschaltungSekunden); Serial.flush();
    // Serial.print("Signalton:                       "); Serial.println(config.signaltonOn ? "Ein" : "Aus"); Serial.flush();
}

// Konfiguration ins EEPROM schreiben
void SystemConfigManager::saveConfig() {
    uint16_t newChecksum = calculateChecksum();
    if (config.checksum != newChecksum) {
        config.checksum = newChecksum;
        writeConfigToEEPROM();  // 12 Bytes
        // Serial.println("Konfiguration gespeichert."); Serial.flush();
    } else {
        // Serial.println("Keine Änderungen. Speichern übersprungen."); Serial.flush();
    }

    // Serial.print("Manuelle Pumpenleistung:     [%] "); Serial.println(config.manuellePumpenleistung); Serial.flush();
    // Serial.print("Impulse pro Liter:               "); Serial.println(config.impulseProLiter); Serial.flush();
    // Serial.print("Minimale Akkuspannung:      [mV] "); Serial.println(config.minimalspannungAkku); Serial.flush();
    // Serial.print("Anzahl Messwerte Mittelwert:     "); Serial.println(config.anzahlMesswerteMittelwert); Serial.flush();
    // Serial.print("Systemabschaltung:        [Sek.] "); Serial.println(config.systemabschaltungSekunden); Serial.flush();
    // Serial.print("Signalton:                       "); Serial.println(config.signaltonOn ? "Ein" : "Aus"); Serial.flush();
}

// Konfiguration auf Standardwerte zurücksetzen
void SystemConfigManager::resetToDefaults() {
    config.manuellePumpenleistung = 80;
    config.impulseProLiter = 5315;
    config.minimalspannungAkku = 105;
    config.anzahlMesswerteMittelwert = 6;
    config.systemabschaltungSekunden = 180;
    config.signaltonOn = true;
    config.battKalibrierungsfaktor = 30000; // 30000 für 3.0000
    config.checksum = calculateChecksum();

    writeConfigToEEPROM();
    #ifdef DEBUG
    Serial.println("Standardwerte gespeichert.");
    Serial.flush();
    #endif
}

// Prüfen, ob die Daten valide sind
bool SystemConfigManager::isValid() {
    return config.checksum == calculateChecksum();
}

// Zugriff auf die Konfigurationsstruktur
SystemConfig& SystemConfigManager::getConfig() {
    return config;
}


// Prüfsumme berechnen
uint16_t SystemConfigManager::calculateChecksum() {
    uint16_t checksum = 0;
    uint8_t* data = (uint8_t*)&config;
    for (size_t i = 0; i < sizeof(SystemConfig) - sizeof(config.checksum); i++) {
        checksum += data[i];
    }
    return checksum;
}

// Konfiguration in EEPROM schreiben
void SystemConfigManager::writeConfigToEEPROM() {
    EEPROM.put(address, config);
    delay(100);
}

// Konfiguration aus EEPROM lesen
void SystemConfigManager::readConfigFromEEPROM() {
    EEPROM.get(address, config);
    delay(100);
}
