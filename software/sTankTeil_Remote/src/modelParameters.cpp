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

#include "modelParameters.h"

// Konstruktor
ModelParameters::ModelParameters()
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
String ModelParameters::getModelName() const { return modelName; }
TankTypeEnum ModelParameters::getTankType() const { return tankType; }
int ModelParameters::getMenge() const { return menge; }
int ModelParameters::getPumpPwr() const { return pumpPwr; }
int ModelParameters::getPressureDropHoseBreak() const { return pressureDropHoseBreak; }
int ModelParameters::getFuelingLast() const { return fuelingLast; }
int ModelParameters::getFuelingCount() const { return fuelingCount; }
int ModelParameters::getFuelingTotal() const { return fuelingTotal; }
int ModelParameters::getMaxRefuelTime() const { return maxRefuelTime; }
int ModelParameters::getMaxDefuelTime() const { return maxDefuelTime; }
int ModelParameters::getBackFuelTime() const { return backFuelTime; }
int ModelParameters::getAirRemovalTime() const { return airRemovalTime; }
int ModelParameters::getPumpStopEmptyDelay() const { return pumpStopEmptyDelay; }
int ModelParameters::getMessurementDelay() const { return messurementDelay; }
int ModelParameters::getMaxRefuelMl() const { return maxRefuelMl; }
int ModelParameters::getMaxDefuelMl() const { return maxDefuelMl; }
int ModelParameters::getMaxPressure() const { return maxPressure; }
int ModelParameters::getPumpStopPressureDiff() const { return pumpStopPressureDiff; }
int ModelParameters::getPumpStopPressureEmpty() const { return pumpStopPressureEmpty; }
int ModelParameters::getHopperPressure() const { return hopperPressure; }
int ModelParameters::getPumpStopHopperPressureDiff() const { return pumpStopHopperPressureDiff; }

// Setter-Methoden
void ModelParameters::setModelName(const String& name) { modelName = name; }
void ModelParameters::setTankType(TankTypeEnum type) { tankType = type; }
void ModelParameters::setMenge(int value) { menge = value; }
void ModelParameters::setPumpPwr(int value) { pumpPwr = value; }
void ModelParameters::setPressureDropHoseBreak(int value) { pressureDropHoseBreak = value; }
void ModelParameters::setFuelingLast(int value) { fuelingLast = value; }
void ModelParameters::setFuelingCount(int value) { fuelingCount = value; }
void ModelParameters::setFuelingTotal(int value) { fuelingTotal = value; }
void ModelParameters::setMaxRefuelTime(int value) {  maxRefuelTime = value; }
void ModelParameters::setMaxDefuelTime(int value) { maxDefuelTime = value; }
void ModelParameters::setBackFuelTime(int value) { backFuelTime = value; }
void ModelParameters::setAirRemovalTime(int value) { airRemovalTime = value; }
void ModelParameters::setPumpStopEmptyDelay(int value) { pumpStopEmptyDelay = value; }
void ModelParameters::setMessurementDelay(int value) { messurementDelay = value; }
void ModelParameters::setMaxRefuelMl(int value) { maxRefuelMl = value; }
void ModelParameters::setMaxDefuelMl(int value) { maxDefuelMl = value; }
void ModelParameters::setMaxPressure(int value) { maxPressure = value; }
void ModelParameters::setPumpStopPressureDiff(int value) { pumpStopPressureDiff = value; }
void ModelParameters::setPumpStopPressureEmpty(int value) { pumpStopPressureEmpty = value; }
void ModelParameters::setHopperPressure(int value) { hopperPressure = value; }
void ModelParameters::setPumpStopHopperPressureDiff(int value) { pumpStopHopperPressureDiff = value; }

// Aus JSON laden
void ModelParameters::fromJSON(const String& json) {
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

// In JSON speichern
String ModelParameters::toJSON() {
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
bool ModelParameters::saveToLittleFS(const char* filename) {
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
    file.flush();
    file.close();
    Serial.println("Daten in LittleFS gespeichert: " + json);
    return written > 0;
}

// Aus LittleFS laden
bool ModelParameters::loadFromLittleFS(const char* filename) {
    if (!LittleFS.begin()) {
        Serial.println("LittleFS konnte nicht gestartet werden!");
        return false;
    }

    if (!LittleFS.exists(filename)) {
        Serial.println("Datei existiert nicht: " + String(filename));
        return false;
    }

    File file = LittleFS.open(filename, "r");
    if (!file) {
        Serial.println("Fehler beim Öffnen der Datei zum Lesen: " + String(filename));
        return false;
    }

    size_t fileSize = file.size();
    if (fileSize == 0) {
        Serial.println("Datei ist leer: " + String(filename));
        file.close();
        return false;
    }
    if (fileSize > 1024) {
        Serial.println("Datei zu groß: " + String(filename) + " (" + String(fileSize) + " bytes)");
        // Continue anyway but with caution
    }

    String json;
    while (file.available()) {
        json += (char)file.read();
    }
    file.close();

    Serial.println("Daten aus LittleFS geladen (" + String(fileSize) + " bytes): " + json);

    fromJSON(json);
    return true;
}
