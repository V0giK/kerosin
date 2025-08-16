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
    ModelParameters()
        : modelName("I'm the BOSS"),
          tankType(TANK_BEUTEL),
          menge(0),
          pumpPwr(0),
          pressureDropHoseBreak(0),
          fuelingLast(0),
          fuelingCount(0),
          fuelingTotal(0),
          maxRefuelTime(0),
          maxDefuelTime(0),
          backFuelTime(0),
          airRemovalTime(0),
          pumpStopEmptyDelay(0),
          messurementDelay(0),
          maxRefuelMl(0),
          maxDefuelMl(0),
          maxPressure(0),
          pumpStopPressureDiff(0),
          pumpStopPressureEmpty(0),
          hopperPressure(0),
          pumpStopHopperPressureDiff(0) {}

    // Getter-Methoden
    String getModelName() const { return modelName; }
    TankTypeEnum getTankType() const { return tankType; }
    int getMenge() const { return menge; }
    int getPumpPwr() const { return pumpPwr; }
    int getPressureDropHoseBreak() const { return pressureDropHoseBreak; }
    int getFuelingLast() const { return fuelingLast; }
    int getFuelingCount() const { return fuelingCount; }
    int getFuelingTotal() const { return fuelingTotal; }
    int getMaxRefuelTime() const { return maxRefuelTime; }
    int getMaxDefuelTime() const { return maxDefuelTime; }
    int getBackFuelTime() const { return backFuelTime; }
    int getAirRemovalTime() const { return airRemovalTime; }
    int getPumpStopEmptyDelay() const { return pumpStopEmptyDelay; }
    int getMessurementDelay() const { return messurementDelay; }
    int getMaxRefuelMl() const { return maxRefuelMl; }
    int getMaxDefuelMl() const { return maxDefuelMl; }
    int getMaxPressure() const { return maxPressure; }
    int getPumpStopPressureDiff() const { return pumpStopPressureDiff; }
    int getPumpStopPressureEmpty() const { return pumpStopPressureEmpty; }
    int getHopperPressure() const { return hopperPressure; }
    int getPumpStopHopperPressureDiff() const { return pumpStopHopperPressureDiff; }

    // Setter-Methoden
    void setModelName(const String& name) { modelName = name; }
    void setTankType(TankTypeEnum type) { tankType = type; }
    void setMenge(int value) { menge = value; }
    void setPumpPwr(int value) { pumpPwr = value; }
    void setPressureDropHoseBreak(int value) { pressureDropHoseBreak = value; }
    void setFuelingLast(int value) { fuelingLast = value; }
    void setFuelingCount(int value) { fuelingCount = value; }
    void setFuelingTotal(int value) { fuelingTotal = value; }
    void setMaxRefuelTime(int value) {  maxRefuelTime = value; }
    void setMaxDefuelTime(int value) { maxDefuelTime = value; }
    void setBackFuelTime(int value) { backFuelTime = value; }
    void setAirRemovalTime(int value) { airRemovalTime = value; }
    void setPumpStopEmptyDelay(int value) { pumpStopEmptyDelay = value; }
    void setMessurementDelay(int value) { messurementDelay = value; }
    void setMaxRefuelMl(int value) { maxRefuelMl = value; }
    void setMaxDefuelMl(int value) { maxDefuelMl = value; }
    void setMaxPressure(int value) { maxPressure = value; }
    void setPumpStopPressureDiff(int value) { pumpStopPressureDiff = value; }
    void setPumpStopPressureEmpty(int value) { pumpStopPressureEmpty = value; }
    void setHopperPressure(int value) { hopperPressure = value; }
    void setPumpStopHopperPressureDiff(int value) { pumpStopHopperPressureDiff = value; }
    

    // Aus JSON laden
    void fromJSON(const String& json) {
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, json);
        if (error) {
            Serial.println("Fehler beim Parsen des JSON!");
            return;
        }

        modelName = doc["modelName"] | "Default";
        tankType = static_cast<TankTypeEnum>(doc["tankType"] | 0);
        menge = doc["menge"] | 0;
        pumpPwr = doc["pumpPwr"] | 0;
        pressureDropHoseBreak = doc["pressureDropHoseBreak"] | 0;
        fuelingLast = doc["fuelingLast"] | 0;
        fuelingCount = doc["fuelingCount"] | 0;
        fuelingTotal = doc["fuelingTotal"] | 0;
        maxRefuelTime = doc["maxRefuelTime"] | 0;
        maxDefuelTime = doc["maxDefuelTime"] | 0;
        backFuelTime = doc["backFuelTime"] | 0;
        airRemovalTime = doc["airRemovalTime"] | 0;
        pumpStopEmptyDelay = doc["pumpStopEmptyDelay"] | 0;
        messurementDelay = doc["messurementDelay"] | 0;
        maxRefuelMl = doc["maxRefuelMl"] | 0;
        maxDefuelMl = doc["maxDefuelMl"] | 0;
        maxPressure = doc["maxPressure"] | 0;
        pumpStopPressureDiff = doc["pumpStopPressureDiff"] | 0;
        pumpStopPressureEmpty = doc["pumpStopPressureEmpty"] | 0;
        hopperPressure = doc["hopperPressure"] | 0;
        pumpStopHopperPressureDiff = doc["pumpStopHopperPressureDiff"] | 0;
    }

    // JSON-Unterstützung für ESP32
#ifdef ESP32
    // In JSON speichern
    String toJSON() {
        StaticJsonDocument<1024> doc;
        doc["modelName"] = modelName;
        doc["tankType"] = tankType;
        doc["menge"] = menge;
        doc["pumpPwr"] = pumpPwr;
        doc["pressureDropHoseBreak"] = pressureDropHoseBreak;
        doc["fuelingLast"] = fuelingLast;
        doc["fuelingCount"] = fuelingCount;
        doc["fuelingTotal"] = fuelingTotal;
        doc["maxRefuelTime"] = maxRefuelTime;
        doc["maxDefuelTime"] = maxDefuelTime;
        doc["backFuelTime"] = backFuelTime;
        doc["airRemovalTime"] = airRemovalTime;
        doc["pumpStopEmptyDelay"] = pumpStopEmptyDelay;
        doc["messurementDelay"] = messurementDelay;
        doc["maxRefuelMl"] = maxRefuelMl;
        doc["maxDefuelMl"] = maxDefuelMl;
        doc["maxPressure"] = maxPressure;
        doc["pumpStopPressureDiff"] = pumpStopPressureDiff;
        doc["pumpStopPressureEmpty"] = pumpStopPressureEmpty;
        doc["hopperPressure"] = hopperPressure;
        doc["pumpStopHopperPressureDiff"] = pumpStopHopperPressureDiff;

        String json;
        serializeJson(doc, json);
        return json;
    }

    // In LittleFS speichern
    bool saveToLittleFS(const char* filename) {
        if (!LittleFS.begin()) {
            Serial.println("LittleFS konnte nicht gestartet werden!");
            return false;
        }

        File file = LittleFS.open(filename, "w");
        if (!file) {
            Serial.println("Fehler beim Öffnen der Datei zum Schreiben!");
            return false;
        }

        String json = toJSON();
        size_t written = file.print(json);
        file.flush(); // Sicherstellen, dass alle Daten geschrieben werden
        file.close();
        Serial.println("Daten in LittleFS gespeichert: " + json);
        return  written > 0;
    }

    // Aus LittleFS laden
    bool loadFromLittleFS(const char* filename) {
        if (!LittleFS.begin()) {
            Serial.println("LittleFS konnte nicht gestartet werden!");
            return false;
        }

        File file = LittleFS.open(filename, "r");
        if (!file) {
            Serial.println("Fehler beim Öffnen der Datei zum Lesen!");
            return false;
        }

        String json;
        while (file.available()) {
            json += (char)file.read();
        }
        file.close();

        fromJSON(json);
        Serial.println("Daten aus LittleFS geladen: " + json);
        return true;
    }


#endif
};