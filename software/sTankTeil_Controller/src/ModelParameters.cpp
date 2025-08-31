#include "ModelParameters.h"
#include <EEPROM.h>

void ModelParameters::getShortModelname(char buffer[16]) {
    memset(buffer, 0, 16);
    if(modelName.length() > 15) {
        modelName.substring(0, 15).toCharArray(buffer, 15);
    } else {
        modelName.toCharArray(buffer, 16);
    }
}

ModelParameters::ModelParameters()
    : modelName("I'm the BOSS"),
      tankType(TANK_BEUTEL),
      menge(0),
      pumpPwr(0),
      pressureDropHoseBreak(0),
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

String ModelParameters::getModelName() const { return modelName; }
int ModelParameters::getTankType() const { return tankType; }
int ModelParameters::getMenge() const { return menge; }
int ModelParameters::getPumpPwr() const { return pumpPwr; }
int ModelParameters::getPressureDropHoseBreak() const { return pressureDropHoseBreak; }
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

void ModelParameters::setModelName(const String& name) { modelName = name; }
void ModelParameters::setTankType(int type) { tankType = type; }
void ModelParameters::setMenge(int value) { menge = value; }
void ModelParameters::setPumpPwr(int value) { pumpPwr = value; }
void ModelParameters::setPressureDropHoseBreak(int value) { pressureDropHoseBreak = value; }
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

void ModelParameters::printModelParameters() {
    Serial.println("----- Modell Daten -----");
    delay(10);
    Serial.print("Modellname:                     "); Serial.println(getModelName());
    delay(10);
    Serial.print("Tanktype                        "); Serial.println(getTankType());
    delay(10);
    Serial.print("Menge:                          "); Serial.println(getMenge());
    delay(10);
    Serial.print("Pump PWR                        "); Serial.println(getPumpPwr());
    delay(10);
    Serial.print("Pressure drop hose break:       "); Serial.println(getPressureDropHoseBreak());
    delay(10);
    Serial.print("Max refuel time:                "); Serial.println(getMaxRefuelTime());
    delay(10);
    Serial.print("Max defuel time:                "); Serial.println(getMaxDefuelTime());
    delay(10);
    Serial.print("Back fuel time:                 "); Serial.println(getBackFuelTime());
    delay(10);
    Serial.print("Air removal time:               "); Serial.println(getAirRemovalTime());
    delay(10);
    Serial.print("Pump stop empty delay:          "); Serial.println(getPumpStopEmptyDelay());
    delay(10);
    Serial.print("Messurement delay:              "); Serial.println(getMessurementDelay());
    delay(10);
    Serial.print("Max refuel ml:                  "); Serial.println(getMaxRefuelMl());
    delay(10);
    Serial.print("Max defuel ml:                  "); Serial.println(getMaxDefuelMl());
    delay(10);
    Serial.print("Max pressure:                   "); Serial.println(getMaxPressure());
    delay(10);
    Serial.print("Pump stop pressure diff:        "); Serial.println(getPumpStopPressureDiff());
    delay(10);
    Serial.print("Pump stop pressure empty:       "); Serial.println(getPumpStopPressureEmpty());
    delay(10);
    Serial.print("Hopper pressure:                "); Serial.println(getHopperPressure());
    delay(10);
    Serial.print("Pump stop hopper pressure diff: "); Serial.println(getPumpStopHopperPressureDiff());
    delay(10);
}

void ModelParameters::fromJSON(const String& json) {
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        Serial.println("Fehler beim Parsen des JSON!");
        return;
    }

    modelName = doc["modelName"] | "Default";
    tankType = doc["tankType"] | 0;
    menge = doc["menge"] | 0;
    pumpPwr = doc["pumpPwr"] | 0;
    pressureDropHoseBreak = doc["pressureDropHoseBreak"] | 0;
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

#ifdef ESP32
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
    file.print(json);
    file.close();
    Serial.println("Daten in LittleFS gespeichert: " + json);
    return true;
}

bool ModelParameters::loadFromLittleFS(const char* filename) {
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

void ModelParameters::writeModelDataToCharArray(char output[MODEL_DATA_SIZE]) {
    memset(output, 0, MODEL_DATA_SIZE);
    getShortModelname(output);

    auto writeIntToBuffer = [](int value, char* buffer) {
        buffer[0] = value & 0xFF;
        buffer[1] = (value >> 8) & 0xFF;
    };

    size_t index = 15;
    writeIntToBuffer(getTankType(), &output[index]); index += 2;
    writeIntToBuffer(getMenge(), &output[index]); index += 2;
    writeIntToBuffer(getPumpPwr(), &output[index]); index += 2;
    writeIntToBuffer(getPressureDropHoseBreak(), &output[index]); index += 2;
    writeIntToBuffer(getMaxRefuelTime(), &output[index]); index += 2;
    writeIntToBuffer(getMaxDefuelTime(), &output[index]); index += 2;
    writeIntToBuffer(getBackFuelTime(), &output[index]); index += 2;
    writeIntToBuffer(getAirRemovalTime(), &output[index]); index += 2;
    writeIntToBuffer(getPumpStopEmptyDelay(), &output[index]); index += 2;
    writeIntToBuffer(getMessurementDelay(), &output[index]); index += 2;
    writeIntToBuffer(getMaxRefuelMl(), &output[index]); index += 2;
    writeIntToBuffer(getMaxDefuelMl(), &output[index]); index += 2;
    writeIntToBuffer(getMaxPressure(), &output[index]); index += 2;
    writeIntToBuffer(getPumpStopPressureDiff(), &output[index]); index += 2;
    writeIntToBuffer(getPumpStopPressureEmpty(), &output[index]); index += 2;
    writeIntToBuffer(getHopperPressure(), &output[index]); index += 2;
    writeIntToBuffer(getPumpStopHopperPressureDiff(), &output[index]); index += 2;
}

uint16_t ModelParameters::calcCkecksum() {
    char modeldata[MODEL_DATA_SIZE];
    writeModelDataToCharArray(modeldata);
    return calculateCRC(modeldata);
}

void ModelParameters::saveToEEPROM(int startAddress) {
    int address = startAddress;
    char buffer[16];
    getShortModelname(buffer);
    EEPROM.put(address, buffer); address += 15 + 1;
    EEPROM.put(address, tankType); address += sizeof(tankType);
    EEPROM.put(address, menge); address += sizeof(menge);
    EEPROM.put(address, pumpPwr); address += sizeof(pumpPwr);
    EEPROM.put(address, pressureDropHoseBreak); address += sizeof(pressureDropHoseBreak);
    EEPROM.put(address, maxRefuelTime); address += sizeof(maxRefuelTime);
    EEPROM.put(address, maxDefuelTime); address += sizeof(maxDefuelTime);
    EEPROM.put(address, backFuelTime); address += sizeof(backFuelTime);
    EEPROM.put(address, airRemovalTime); address += sizeof(airRemovalTime);
    EEPROM.put(address, pumpStopEmptyDelay); address += sizeof(pumpStopEmptyDelay);
    EEPROM.put(address, messurementDelay); address += sizeof(messurementDelay);
    EEPROM.put(address, maxRefuelMl); address += sizeof(maxRefuelMl);
    EEPROM.put(address, maxDefuelMl); address += sizeof(maxDefuelMl);
    EEPROM.put(address, maxPressure); address += sizeof(maxPressure);
    EEPROM.put(address, pumpStopPressureDiff); address += sizeof(pumpStopPressureDiff);
    EEPROM.put(address, pumpStopPressureEmpty); address += sizeof(pumpStopPressureEmpty);
    EEPROM.put(address, hopperPressure); address += sizeof(hopperPressure);
    EEPROM.put(address, pumpStopHopperPressureDiff); address += sizeof(pumpStopHopperPressureDiff);
    uint16_t checksum = calcCkecksum();
    EEPROM.put(address, checksum); address += sizeof(checksum);
}

bool ModelParameters::loadFromEEPROM(int startAddress) {
    int address = startAddress;
    char buffer[16];
    memset(buffer, 0, 16);
    EEPROM.get(address, buffer); modelName = String(buffer); address += 15 + 1;
    EEPROM.get(address, tankType); address += sizeof(tankType);
    EEPROM.get(address, menge); address += sizeof(menge);
    EEPROM.get(address, pumpPwr); address += sizeof(pumpPwr);
    EEPROM.get(address, pressureDropHoseBreak); address += sizeof(pressureDropHoseBreak);
    EEPROM.get(address, maxRefuelTime); address += sizeof(maxRefuelTime);
    EEPROM.get(address, maxDefuelTime); address += sizeof(maxDefuelTime);
    EEPROM.get(address, backFuelTime); address += sizeof(backFuelTime);
    EEPROM.get(address, airRemovalTime); address += sizeof(airRemovalTime);
    EEPROM.get(address, pumpStopEmptyDelay); address += sizeof(pumpStopEmptyDelay);
    EEPROM.get(address, messurementDelay); address += sizeof(messurementDelay);
    EEPROM.get(address, maxRefuelMl); address += sizeof(maxRefuelMl);
    EEPROM.get(address, maxDefuelMl); address += sizeof(maxDefuelMl);
    EEPROM.get(address, maxPressure); address += sizeof(maxPressure);
    EEPROM.get(address, pumpStopPressureDiff); address += sizeof(pumpStopPressureDiff);
    EEPROM.get(address, pumpStopPressureEmpty); address += sizeof(pumpStopPressureEmpty);
    EEPROM.get(address, hopperPressure); address += sizeof(hopperPressure);
    EEPROM.get(address, pumpStopHopperPressureDiff); address += sizeof(pumpStopHopperPressureDiff);
    uint16_t checksum;
    EEPROM.get(address, checksum); address += sizeof(checksum);

    return (calcCkecksum() == checksum && checksum != 0);
}

void ModelParameters::clearCRCeeprom(int startAddress){
    int address = startAddress;
    address += 15 + 1;
    address += sizeof(tankType);
    address += sizeof(menge);
    address += sizeof(pumpPwr);
    address += sizeof(pressureDropHoseBreak);
    address += sizeof(maxRefuelTime);
    address += sizeof(maxDefuelTime);
    address += sizeof(backFuelTime);
    address += sizeof(airRemovalTime);
    address += sizeof(pumpStopEmptyDelay);
    address += sizeof(messurementDelay);
    address += sizeof(maxRefuelMl);
    address += sizeof(maxDefuelMl);
    address += sizeof(maxPressure);
    address += sizeof(pumpStopPressureDiff);
    address += sizeof(pumpStopPressureEmpty);
    address += sizeof(hopperPressure);
    address += sizeof(pumpStopHopperPressureDiff);
    uint16_t checksum = (uint16_t)999999;
    EEPROM.put(address, checksum); address += sizeof(checksum);
}