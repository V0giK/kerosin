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
    VoltageReader(int pin, float refVoltage, int readings)
        : analogPin(pin), referenceVoltage(refVoltage), numReadings(readings),
          calibrationFactor(1.0), lastVoltage(-1), total(0),
          readingsCount(0), lastReadTime(0), readInterval(10),
          warningVoltage(0), emptyVoltage(0), callback(nullptr),
          warningStartTime(0), emptyStartTime(0),
          isBelowWarning(false), isBelowEmpty(false) {}

    void setCalibrationFactor(float calibFactor) {
        calibrationFactor = calibFactor;
    }

    void setThresholds(float warning, float empty, void (*cb)(const char*, unsigned long)) {
        warningVoltage = warning;
        emptyVoltage = empty;
        callback = cb;
    }

    void readVoltage() {
        if (millis() - lastReadTime >= readInterval) {
            lastReadTime = millis();
            total += analogRead(analogPin);
            readingsCount++;
            Serial.print("Raw ADC: "); 
            Serial.print(" | Voltage: ");
            Serial.print(analogRead(analogPin));
            Serial.print(" | Reference Voltage: ");
            Serial.print(referenceVoltage);
            Serial.print(" | Calibration Factor: ");
            Serial.print(calibrationFactor);
            Serial.print(" | Calibrated Voltage: ");
            Serial.println((analogRead(analogPin) / 1023.0) * referenceVoltage * calibrationFactor);
           }

        if (readingsCount >= numReadings) {
            float average = total / numReadings;
            lastVoltage = (average / 1023.0) * referenceVoltage * calibrationFactor;

            total = 0;
            readingsCount = 0;
        }
    }

    void evaluateVoltage() {
        if (lastVoltage < emptyVoltage) {
            if (!isBelowEmpty) {
                emptyStartTime = millis();
                isBelowEmpty = true;
            }
            if (callback) {
                callback("empty", (millis() - emptyStartTime) / 1000);
            }
        } else {
            isBelowEmpty = false;
        }

        if (lastVoltage < warningVoltage && lastVoltage >= emptyVoltage) {
            if (!isBelowWarning) {
                warningStartTime = millis();
                isBelowWarning = true;
            }
            if (callback) {
                callback("warning", (millis() - warningStartTime) / 1000);
            }
        } else {
            isBelowWarning = false;
        }
    }

    float getLastVoltage() const {
        return lastVoltage;
    }

    String getLastVoltageString(uint8_t nachkommastellen = 1) {
        char buffer[16];
        dtostrf(lastVoltage, 4, nachkommastellen, buffer);
        return String(buffer);
    }
};
