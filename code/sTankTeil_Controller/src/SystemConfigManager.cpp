#include "SystemConfigManager.h"

// Konstruktor
SystemConfigManager::SystemConfigManager(int eepromAddress)
    : address(eepromAddress) {}

// Konfiguration aus EEPROM laden
void SystemConfigManager::loadConfig() {
    readConfigFromEEPROM();

    if (!isValid()) {
        Serial.println("Ungültige Konfiguration. Setze auf Standardwerte...");
        delay(10);
        resetToDefaults();
    } else {
        Serial.println("Konfiguration erfolgreich geladen.");
        delay(10);
    }

    // Serial.print("Manuelle Pumpenleistung:     [%] "); Serial.println(config.manuellePumpenleistung);
    // Serial.print("Impulse pro Liter:               "); Serial.println(config.impulseProLiter);
    // Serial.print("Minimale Akkuspannung:      [mV] "); Serial.println(config.minimalspannungAkku);
    // Serial.print("Anzahl Messwerte Mittelwert:     "); Serial.println(config.anzahlMesswerteMittelwert);
    // Serial.print("Systemabschaltung:        [Sek.] "); Serial.println(config.systemabschaltungSekunden);
    // Serial.print("Signalton:                       "); Serial.println(config.signaltonOn ? "Ein" : "Aus");
}

// Konfiguration ins EEPROM schreiben
void SystemConfigManager::saveConfig() {
    uint16_t newChecksum = calculateChecksum();
    if (config.checksum != newChecksum) {
        config.checksum = newChecksum;
        writeConfigToEEPROM();  // 12 Bytes
        // Serial.println("Konfiguration gespeichert.");
        delay(10);
    } else {
        // Serial.println("Keine Änderungen. Speichern übersprungen.");
        delay(10);
    }

    // Serial.print("Manuelle Pumpenleistung:     [%] "); Serial.println(config.manuellePumpenleistung);
    // Serial.print("Impulse pro Liter:               "); Serial.println(config.impulseProLiter);
    // Serial.print("Minimale Akkuspannung:      [mV] "); Serial.println(config.minimalspannungAkku);
    // Serial.print("Anzahl Messwerte Mittelwert:     "); Serial.println(config.anzahlMesswerteMittelwert);
    // Serial.print("Systemabschaltung:        [Sek.] "); Serial.println(config.systemabschaltungSekunden);
    // Serial.print("Signalton:                       "); Serial.println(config.signaltonOn ? "Ein" : "Aus");
}

// Konfiguration auf Standardwerte zurücksetzen
void SystemConfigManager::resetToDefaults() {
    config.manuellePumpenleistung = 80;
    config.impulseProLiter = 5315;
    config.minimalspannungAkku = 105;
    config.anzahlMesswerteMittelwert = 6;
    config.systemabschaltungSekunden = 180;
    config.signaltonOn = true;
    config.checksum = calculateChecksum();

    writeConfigToEEPROM();
    Serial.println("Standardwerte gespeichert.");
    delay(10);
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
