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

#include "Arduino.h"

class VoltageReader {
private:
    int analogPin;
    float referenceVoltage;
    int numReadings;
    float calibrationFactor;
    float lastVoltage;
    long total;
    int readingsCount;
    unsigned long lastReadTime;
    unsigned long readInterval;
    float warningVoltage;
    float emptyVoltage;
    void (*callback)(const char*, unsigned long);
    unsigned long warningStartTime;
    unsigned long emptyStartTime;
    bool isBelowWarning;
    bool isBelowEmpty;

public:
    VoltageReader(int pin, float refVoltage, int readings);

    void setCalibrationFactor(float calibFactor);
    void setThresholds(float warning, float empty, void (*cb)(const char*, unsigned long));
    void readVoltage();
    void evaluateVoltage();
    float getLastVoltage() const;
    String getLastVoltageString(uint8_t nachkommastellen = 1);
};