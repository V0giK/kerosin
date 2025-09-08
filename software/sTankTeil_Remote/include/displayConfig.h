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
#include "lovyanGfxSetup.h"
#include "lv_conf.h"

#define TFT_HOR_RES SCREEN_WIDTH
#define TFT_VER_RES SCREEN_HEIGHT

// Reduziere Buffergröße für weniger RAM-Verbrauch
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 40 * (LV_COLOR_DEPTH / 8))

// Buffer als static deklarieren, optional in PSRAM für ESP32 (Weniger internal RAM-Verbrauch)
#ifdef BOARD_HAS_PSRAM
#include <esp_attr.h>  // Für ESP32 PSRAM-Attribut
EXT_RAM_ATTR static uint32_t draw_buf[DRAW_BUF_SIZE / 4] = {0};
#else
static uint32_t draw_buf[DRAW_BUF_SIZE / 4] = {0};
#endif
