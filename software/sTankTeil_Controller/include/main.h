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
#include "variables.h"

// Nano resetten
void(* resetNano) (void) = 0; // Deklaration eines Funktionszeigers auf Adresse 0

void onButtonInClick();
void onButtonOutClick();
void onBothButtonsLongPress();

void handleFlowMeasurement();
void handlePressureMeasurement();

void handlePumpSeq();


// Callbacks
void onWrite(int16_t id, const String &value);
void onRead(int16_t id);