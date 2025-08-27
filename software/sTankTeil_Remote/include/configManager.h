#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

struct Config {
    String owner;
    int lastModel;
    int beep;
    int flowTicks;
    int pressureAvg;
    int akkuMinV;
    int akkuFactor;
    int sysPowerOffTime;
    int pumpPwrManu;
    int pumpPwrCalib;
};

bool loadConfig(Config &config, const char *configFilePath);
bool saveConfig(const Config &config, const char *configFilePath);
void loadConfigWithDefaults(Config &config, const char *configFilePath);
void applyConfigToUI(const Config &config);
void printSystemSettings(const Config &config);
