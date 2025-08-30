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

class LEDController {
private:
    int pin;
    unsigned long lastBlinkTime;
    unsigned long blinkIntervalLowBattery;
    unsigned long blinkIntervalError;
    bool isBlinkingLowBattery;
    bool isBlinkingError;
    bool ledState;

public:
    LEDController(int ledPin);

    void startLowBatteryBlinking();
    void stopLowBatteryBlinking();
    void startErrorBlinking();
    void stopErrorBlinking();
    void update();

private:
    void updateLEDState();
};