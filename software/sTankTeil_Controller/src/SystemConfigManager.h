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
#ifndef SYSTEMCONFIGMANAGER_H
#define SYSTEMCONFIGMANAGER_H

#include <EEPROM.h>
#include <Arduino.h>

// Konfigurationsstruktur - 12 Bytes
struct SystemConfig {
    int manuellePumpenleistung;          // 3-stellig, zb. 100 (%)
    int impulseProLiter;                 // 5-stellig, zb. 5314
    int minimalspannungAkku;             // 3-stellig, zb. 111(11.1V)
    uint8_t anzahlMesswerteMittelwert;   // 2-stellig, zb. 6
    int systemabschaltungSekunden;       // 3-stellig, zb. 180 (Sekunden) - 0=keine Abschaltung
    bool signaltonOn;                    // 1-stellig (true/false)
    uint16_t checksum;                   // Prüfsumme für Datenvalidierung
};

class SystemConfigManager {
public:
    // Konstruktor
    SystemConfigManager(int eepromAddress);

    // Methoden
    void loadConfig();                 // Konfiguration aus EEPROM laden
    void saveConfig();                 // Konfiguration ins EEPROM schreiben
    void resetToDefaults();            // Konfiguration auf Standardwerte zurücksetzen
    bool isValid();                    // Prüfen, ob die Daten valide sind

    void setManuellePumpenleistung(int value) { config.manuellePumpenleistung = value; }
    void setImpulseProLiter(int value) { config.impulseProLiter = value; }
    void setMinimalspannungAkku(int value) { config.minimalspannungAkku = value; }
    void setAnzahlMesswerteMittelwert(uint8_t value) { config.anzahlMesswerteMittelwert = value; }
    void setSystemabschaltungSekunden(int value) { config.systemabschaltungSekunden = (value>60||value==0)?value:60; }
    void setSignaltonOn(bool value) { config.signaltonOn = value; }
    //void setChecksum(uint16_t value) { config.checksum = value; }

    // Getter-Methoden
    int getManuellePumpenleistung() const { return config.manuellePumpenleistung; }
    int getImpulseProLiter() const { return config.impulseProLiter; }
    int getMinimalspannungAkku() const { return config.minimalspannungAkku; }
    uint8_t getAnzahlMesswerteMittelwert() const { return config.anzahlMesswerteMittelwert; }
    int getSystemabschaltungSekunden() const { return config.systemabschaltungSekunden; }
    bool getSignaltonOn() const { return config.signaltonOn; }
    //uint16_t getChecksum() const { return config.checksum; }

private:
    SystemConfig config;               // Konfigurationsstruktur
    const int address;                 // EEPROM-Startadresse

    SystemConfig& getConfig();         // Zugriff auf die Konfigurationswerte

    uint16_t calculateChecksum();      // Prüfsumme berechnen
    void writeConfigToEEPROM();        // In EEPROM schreiben
    void readConfigFromEEPROM();       // Aus EEPROM lesen
};

#endif
