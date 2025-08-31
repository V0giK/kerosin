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
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "helper.h"
#ifdef ESP32
#include <LittleFS.h>
#else
#include "defines.h"
#endif

// Klasse für Modellparameter
class ModelParameters {
private:
    String modelName;
    int tankType;
    int menge;
    int pumpPwr;
    int pressureDropHoseBreak;
    int maxRefuelTime;
    int maxDefuelTime;
    int backFuelTime;
    int airRemovalTime;
    int pumpStopEmptyDelay;
    int messurementDelay;
    int maxRefuelMl;
    int maxDefuelMl;
    int maxPressure;
    int pumpStopPressureDiff;
    int pumpStopPressureEmpty;
    int hopperPressure;
    int pumpStopHopperPressureDiff;

    void getShortModelname(char buffer[16]);
    // 15 Bytes für modelName + 20 * 2 Bytes für die int-Werte = 55 Bytes
    static const size_t MODEL_DATA_SIZE = 15 + (20 * 2);

public:
    // Konstruktor
    ModelParameters();

    // Getter-Methoden
    String getModelName() const;
    int getTankType() const;
    int getMenge() const;
    int getPumpPwr() const;
    int getPressureDropHoseBreak() const;
    int getMaxRefuelTime() const;
    int getMaxDefuelTime() const;
    int getBackFuelTime() const;
    int getAirRemovalTime() const;
    int getPumpStopEmptyDelay() const;
    int getMessurementDelay() const;
    int getMaxRefuelMl() const;
    int getMaxDefuelMl() const;
    int getMaxPressure() const;
    int getPumpStopPressureDiff() const;
    int getPumpStopPressureEmpty() const;
    int getHopperPressure() const;
    int getPumpStopHopperPressureDiff() const;

    // Setter-Methoden
    void setModelName(const String& name);
    void setTankType(int type);
    void setMenge(int value);
    void setPumpPwr(int value);
    void setPressureDropHoseBreak(int value);
    void setFuelingLast(int value);
    void setFuelingCount(int value);
    void setFuelingTotal(int value);
    void setMaxRefuelTime(int value);
    void setMaxDefuelTime(int value);
    void setBackFuelTime(int value);
    void setAirRemovalTime(int value);
    void setPumpStopEmptyDelay(int value);
    void setMessurementDelay(int value);
    void setMaxRefuelMl(int value);
    void setMaxDefuelMl(int value);
    void setMaxPressure(int value);
    void setPumpStopPressureDiff(int value);
    void setPumpStopPressureEmpty(int value);
    void setHopperPressure(int value);
    void setPumpStopHopperPressureDiff(int value);


    void printModelParameters();
    void fromJSON(const String& json);

#ifdef ESP32
    String toJSON();
    bool saveToLittleFS(const char* filename);
    bool loadFromLittleFS(const char* filename);
#endif

    void writeModelDataToCharArray(char output[MODEL_DATA_SIZE]);
    uint16_t calcCkecksum();
    void saveToEEPROM(int startAddress);
    bool loadFromEEPROM(int startAddress);
    void clearCRCeeprom(int startAddress);
};