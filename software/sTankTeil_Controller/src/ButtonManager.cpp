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

#include "ButtonManager.h"

ButtonManager::ButtonManager(int pin1, int pin2, unsigned long longPressDuration)
    : button1Pin(pin1), button2Pin(pin2), longPressDuration(longPressDuration), bothButtonsPressed(false), pressStartTime(0),
      longPressEventTriggered(false), bActive(false), button1(), button2(),
      button1ClickCallback(nullptr), button2ClickCallback(nullptr), bothButtonsLongPressCallback(nullptr) {
    button1.attach(button1Pin);
    button1.interval(10);
    button2.attach(button2Pin);
    button2.interval(10);
}

void ButtonManager::begin() {
    pinMode(button1Pin, INPUT);
    pinMode(button2Pin, INPUT);
    bActive = true;
}

void ButtonManager::onButtonInClick(Callback callback) {
    button1ClickCallback = callback;
}

void ButtonManager::onButtonOutClick(Callback callback) {
    button2ClickCallback = callback;
}

void ButtonManager::onBothButtonsLongPress(Callback callback) {
    bothButtonsLongPressCallback = callback;
}

void ButtonManager::update() {
    if(!bActive) return;

    button1.update();
    button2.update();

    if (button1.read() == LOW && button2.read() == LOW) {
        if (!bothButtonsPressed) {
            pressStartTime = millis();
            bothButtonsPressed = true;
        }

        if (millis() - pressStartTime >= longPressDuration) {
            if (!longPressEventTriggered && bothButtonsLongPressCallback) {
                bothButtonsLongPressCallback();
                longPressEventTriggered = true;
            }
        }
    } else {
        if (bothButtonsPressed) {
            if(button1.read() == HIGH && button2.read() == HIGH) {
                bothButtonsPressed = false;
                longPressEventTriggered = false;
            }
        } else {
            if (!longPressEventTriggered) {
                if (button1.rose() && button1ClickCallback) {
                    button1ClickCallback();
                }
                if (button2.rose() && button2ClickCallback) {
                    button2ClickCallback();
                }
            }
        }
    }
}
