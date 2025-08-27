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

#include <lvgl.h>
#include "lovyanGfxSetup.h"

extern LGFX tft;
extern const bool DEBUG;

// Funktionsprototypen
void deleteModel(lv_obj_t *obj);
// New function prototypes for setup improvements
bool initializeFileSystem();
void handleControllerError();
void navigateToInitialScreen();
