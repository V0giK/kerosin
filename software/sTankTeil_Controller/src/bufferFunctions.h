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
#include "defines.h"
#include "variables.h"
#include "pumpFunctions.h"

// Strukturdefinition
struct recData {
    int16_t id;
    String value;
};

// FIFO Buffer
extern CircularBuffer<recData, 15> fifo_input_buffer;

// Empfangspuffer verarbeiten
recData getRecDataObj(int16_t id, String value);
bool buffer_tick();