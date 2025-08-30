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

#include <Bounce2.h>

class ButtonManager {
public:
    // Typdefinition für die Callback-Funktionen
    typedef void (*Callback)();

    ButtonManager(int pin1, int pin2, unsigned long longPressDuration = 2000);

    void begin();
    void onButtonInClick(Callback callback);
    void onButtonOutClick(Callback callback);
    void onBothButtonsLongPress(Callback callback);
    void update();

private:
    int button1Pin, button2Pin;
    unsigned long longPressDuration;
    bool bothButtonsPressed;
    unsigned long pressStartTime;
    bool longPressEventTriggered;
    bool bActive;
    Bounce button1;
    Bounce button2;
    Callback button1ClickCallback;
    Callback button2ClickCallback;
    Callback bothButtonsLongPressCallback;
};
