#include "configManager.h"
#include "main.h" // <-- Hinzufügen, damit extern DEBUG gefunden wird
#include "ui/vars.h"
#include "helper.h"

// extern DEBUG-Variable aus main.cpp verwenden
extern const bool DEBUG;

bool saveConfig(const Config &config, const char *configFilePath) {
    File file = LittleFS.open(configFilePath, "w");
    if (!file) {
        if (DEBUG) Serial.println("[configManager] Konfigurationsdatei konnte nicht erstellt werden!");
        return false;
    }

    StaticJsonDocument<512> doc;
    doc["owner"] = config.owner;
    doc["lastModel"] = config.lastModel;
    doc["beep"] = config.beep;
    doc["flowTicks"] = config.flowTicks;
    doc["pressureAvg"] = config.pressureAvg;
    doc["akkuMinV"] = config.akkuMinV;
    doc["sysPowerOffTime"] = config.sysPowerOffTime;
    doc["pumpPwrManu"] = config.pumpPwrManu;
    doc["pumpPwrCalib"] = config.pumpPwrCalib;

    if (serializeJson(doc, file) == 0) {
        if (DEBUG) Serial.println("[configManager] JSON-Serialisierung fehlgeschlagen!");
        file.close();
        return false;
    }

    file.close();
    if (DEBUG) Serial.println("[configManager] Konfiguration erfolgreich gespeichert!");
    return true;
}

bool loadConfig(Config &config, const char *configFilePath) {
    File file = LittleFS.open(configFilePath, "r");
    if (!file) {
        if (DEBUG) Serial.println("[configManager] Konfigurationsdatei konnte nicht geöffnet werden!");
        return false;
    }

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        if (DEBUG) Serial.println("[configManager] JSON-Parsing fehlgeschlagen!");
        file.close();
        return false;
    }

    config.owner = doc["owner"].as<String>();
    config.lastModel = doc["lastModel"];
    config.beep = doc["beep"];
    config.flowTicks = doc["flowTicks"];
    config.pressureAvg = doc["pressureAvg"];
    config.akkuMinV = doc["akkuMinV"];
    config.sysPowerOffTime = doc["sysPowerOffTime"];
    config.pumpPwrManu = doc["pumpPwrManu"];
    config.pumpPwrCalib = doc["pumpPwrCalib"];

    file.close();
    if (DEBUG) Serial.println("[configManager] Konfiguration erfolgreich geladen!");
    return true;
}

void loadConfigWithDefaults(Config &config, const char *configFilePath) {
    if (!loadConfig(config, configFilePath)) {
        if (DEBUG) Serial.println("[configManager] Lade Defaults für Config...");
        config.owner = "I'm the BOSS";
        config.lastModel = 0;
        config.beep = 1;
        config.flowTicks = 5315;
        config.pressureAvg = 6;
        config.akkuMinV = 105;
        config.sysPowerOffTime = 180;
        config.pumpPwrManu = 80;
        config.pumpPwrCalib = 80;
        saveConfig(config, configFilePath);
    }
}

void applyConfigToUI(const Config &config) {
    set_var_s_owner(config.owner.c_str());
    set_var_b_load_last_model(!(config.lastModel == 0));
    set_var_b_signal((config.beep == 1));
    set_var_s_flow_ticks(int2char(config.flowTicks));
    set_var_s_pressure_avg(int2char(config.pressureAvg));
    set_var_s_akku_min_v(String(config.akkuMinV / 10.0).c_str());
    // akkuFactor wird auf dem Controller (Nano) berechnet und dort im EEPROM gespeichert
    set_var_s_sys_power_off_time(int2char(config.sysPowerOffTime));
    set_var_s_pump_pwr_manu(int2char(config.pumpPwrManu));
    set_var_s_pump_pwr_calib(int2char(config.pumpPwrCalib));
    set_var_i_pump_pwr(config.pumpPwrCalib);
    set_var_s_pump_pwr(int2char(get_var_i_pump_pwr(), LBL_POSTFIX_PROZENT));
}

// Debug-Ausgabe der Systemsettings
void printSystemSettings(const Config &config) {
    Serial.println("Owner:                " + config.owner);
    Serial.println("Load last model:      " + String(!(config.lastModel == 0)));
    Serial.println("Signalton:            " + String(config.beep));
    Serial.println("Flow Ticks:           " + String(config.flowTicks));
    Serial.println("Flow Messungen AVG:   " + String(config.pressureAvg));
    Serial.println("Akku min. V:          " + String(config.akkuMinV));
    Serial.println("System idle off:      " + String(config.sysPowerOffTime));
    Serial.println("PWR manuell:          " + String(config.pumpPwrManu));
    Serial.println("PWR Calib:            " + String(config.pumpPwrCalib));
    Serial.println();
}
