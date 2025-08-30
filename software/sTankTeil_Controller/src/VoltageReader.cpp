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

#include "VoltageReader.h"

VoltageReader::VoltageReader(int pin, float refVoltage, int readings)
    : analogPin(pin), referenceVoltage(refVoltage), numReadings(readings),
      calibrationFactor(1.0), lastVoltage(-1), total(0),
      readingsCount(0), lastReadTime(0), readInterval(10),
      warningVoltage(0), emptyVoltage(0), callback(nullptr),
      warningStartTime(0), emptyStartTime(0),
      isBelowWarning(false), isBelowEmpty(false) {}

void VoltageReader::setCalibrationFactor(float calibFactor) {
    calibrationFactor = calibFactor;
}

void VoltageReader::setThresholds(float warning, float empty, void (*cb)(const char*, unsigned long)) {
    warningVoltage = warning;
    emptyVoltage = empty;
    callback = cb;
}

void VoltageReader::readVoltage() {
    if (millis() - lastReadTime >= readInterval) {
        lastReadTime = millis();
        total += analogRead(analogPin);
        readingsCount++;
    }

    if (readingsCount >= numReadings) {
        float average = total / numReadings;
        lastVoltage = (average / 1023.0) * referenceVoltage * calibrationFactor;

        total = 0;
        readingsCount = 0;
    }
}

void VoltageReader::evaluateVoltage() {
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

float VoltageReader::getLastVoltage() const {
    return lastVoltage;
}

String VoltageReader::getLastVoltageString(uint8_t nachkommastellen) {
    char buffer[16];
    dtostrf(lastVoltage, 4, nachkommastellen, buffer);
    return String(buffer);
}
