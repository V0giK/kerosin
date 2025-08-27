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
#ifdef ESP32
#include "helper.h"
#include <LittleFS.h> // JSON-Unterstützung nur für ESP32
#else
#include "defines.h"
#endif


// Klasse für Modellparameter
class ModelParameters {
private:
    String modelName;
    TankTypeEnum tankType;
    int menge;
    int pumpPwr;
    int pressureDropHoseBreak;
    int fuelingLast;
    int fuelingCount;
    int fuelingTotal;
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

public:
    // Konstruktor
    ModelParameters();

    // Getter-Methoden
    String getModelName() const;
    TankTypeEnum getTankType() const;
    int getMenge() const;
    int getPumpPwr() const;
    int getPressureDropHoseBreak() const;
    int getFuelingLast() const;
    int getFuelingCount() const;
    int getFuelingTotal() const;
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
    void setTankType(TankTypeEnum type);
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

    // Aus JSON laden
    void fromJSON(const String& json);

    // In JSON speichern
    String toJSON();
    // In LittleFS speichern
    bool saveToLittleFS(const char* filename);
    // Aus LittleFS laden
    bool loadFromLittleFS(const char* filename);
};