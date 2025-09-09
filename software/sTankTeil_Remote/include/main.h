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

// Complete UIFlags typedef that works with the existing codebase
#include <stdint.h>

typedef union UIFlags {
    struct {
        uint32_t g_go2home :1;
        uint32_t g_go2settings :1;
        uint32_t g_keyboardShow :1;
        uint32_t g_numpadShow :1;
        uint32_t g_go2settingsSystem :1;
        uint32_t g_go2settingsCalibrate :1;
        uint32_t g_go2settingsModels :1;
        uint32_t g_unloadManuelFuel :1;
        uint32_t g_pumpStop :1;
        uint32_t g_pumpIn :1;
        uint32_t g_pumpOut :1;
        uint32_t g_go2model :1;
        uint32_t g_go2manuelPump :1;
        uint32_t g_go2calibrateModel :1;
        uint32_t g_unloadSystemSettings :1;
        uint32_t g_eventButtonClick :1;
        uint32_t g_changePumpPwr :1;
        uint32_t g_go2newModel :1;
        uint32_t g_unloadModelSettings :1;
        uint32_t bModelButtonLongPressed :1;
        uint32_t g_viewCalibVolt :1;
    } bits;
    uint32_t raw[2];
} UIFlags;

extern volatile UIFlags g_uiflags;

// Backup mode flags
extern volatile bool inBackupMode;
extern volatile bool otaCompletedFlag;

extern LGFX tft;
extern const bool DEBUG;

// VERSION
#define SW_VERSION "1.1.0-beta"

// UI Flag Macros for better code maintainability
#define UI_SET_GO2HOME() (g_uiflags.bits.g_go2home = 1)
#define UI_CLEAR_GO2HOME() (g_uiflags.bits.g_go2home = 0)
#define UI_IS_GO2HOME() (g_uiflags.bits.g_go2home)

#define UI_SET_GO2SETTINGS() (g_uiflags.bits.g_go2settings = 1)
#define UI_CLEAR_GO2SETTINGS() (g_uiflags.bits.g_go2settings = 0)
#define UI_IS_GO2SETTINGS() (g_uiflags.bits.g_go2settings)

#define UI_SET_KEYBOARD_SHOW() (g_uiflags.bits.g_keyboardShow = 1)
#define UI_CLEAR_KEYBOARD_SHOW() (g_uiflags.bits.g_keyboardShow = 0)
#define UI_IS_KEYBOARD_SHOW() (g_uiflags.bits.g_keyboardShow)

#define UI_SET_NUMPAD_SHOW() (g_uiflags.bits.g_numpadShow = 1)
#define UI_CLEAR_NUMPAD_SHOW() (g_uiflags.bits.g_numpadShow = 0)
#define UI_IS_NUMPAD_SHOW() (g_uiflags.bits.g_numpadShow)

#define UI_SET_GO2SETTINGS_SYSTEM() (g_uiflags.bits.g_go2settingsSystem = 1)
#define UI_CLEAR_GO2SETTINGS_SYSTEM() (g_uiflags.bits.g_go2settingsSystem = 0)
#define UI_IS_GO2SETTINGS_SYSTEM() (g_uiflags.bits.g_go2settingsSystem)

#define UI_SET_GO2SETTINGS_CALIBRATE() (g_uiflags.bits.g_go2settingsCalibrate = 1)
#define UI_CLEAR_GO2SETTINGS_CALIBRATE() (g_uiflags.bits.g_go2settingsCalibrate = 0)
#define UI_IS_GO2SETTINGS_CALIBRATE() (g_uiflags.bits.g_go2settingsCalibrate)

#define UI_SET_GO2SETTINGS_MODELS() (g_uiflags.bits.g_go2settingsModels = 1)
#define UI_CLEAR_GO2SETTINGS_MODELS() (g_uiflags.bits.g_go2settingsModels = 0)
#define UI_IS_GO2SETTINGS_MODELS() (g_uiflags.bits.g_go2settingsModels)

#define UI_SET_UNLOAD_MANUEL_FUEL() (g_uiflags.bits.g_unloadManuelFuel = 1)
#define UI_CLEAR_UNLOAD_MANUEL_FUEL() (g_uiflags.bits.g_unloadManuelFuel = 0)
#define UI_IS_UNLOAD_MANUEL_FUEL() (g_uiflags.bits.g_unloadManuelFuel)

#define UI_SET_PUMP_STOP() (g_uiflags.bits.g_pumpStop = 1)
#define UI_CLEAR_PUMP_STOP() (g_uiflags.bits.g_pumpStop = 0)
#define UI_IS_PUMP_STOP() (g_uiflags.bits.g_pumpStop)

#define UI_SET_PUMP_IN() (g_uiflags.bits.g_pumpIn = 1)
#define UI_CLEAR_PUMP_IN() (g_uiflags.bits.g_pumpIn = 0)
#define UI_IS_PUMP_IN() (g_uiflags.bits.g_pumpIn)

#define UI_SET_PUMP_OUT() (g_uiflags.bits.g_pumpOut = 1)
#define UI_CLEAR_PUMP_OUT() (g_uiflags.bits.g_pumpOut = 0)
#define UI_IS_PUMP_OUT() (g_uiflags.bits.g_pumpOut)

#define UI_SET_GO2MODEL() (g_uiflags.bits.g_go2model = 1)
#define UI_CLEAR_GO2MODEL() (g_uiflags.bits.g_go2model = 0)
#define UI_IS_GO2MODEL() (g_uiflags.bits.g_go2model)

#define UI_SET_GO2MANUEL_PUMP() (g_uiflags.bits.g_go2manuelPump = 1)
#define UI_CLEAR_GO2MANUEL_PUMP() (g_uiflags.bits.g_go2manuelPump = 0)
#define UI_IS_GO2MANUEL_PUMP() (g_uiflags.bits.g_go2manuelPump)

#define UI_SET_UNLOAD_SYSTEM_SETTINGS() (g_uiflags.bits.g_unloadSystemSettings = 1)
#define UI_CLEAR_UNLOAD_SYSTEM_SETTINGS() (g_uiflags.bits.g_unloadSystemSettings = 0)
#define UI_IS_UNLOAD_SYSTEM_SETTINGS() (g_uiflags.bits.g_unloadSystemSettings)

#define UI_SET_EVENT_BUTTON_CLICK() (g_uiflags.bits.g_eventButtonClick = 1)
#define UI_CLEAR_EVENT_BUTTON_CLICK() (g_uiflags.bits.g_eventButtonClick = 0)
#define UI_IS_EVENT_BUTTON_CLICK() (g_uiflags.bits.g_eventButtonClick)

#define UI_SET_CHANGE_PUMP_PWR() (g_uiflags.bits.g_changePumpPwr = 1)
#define UI_CLEAR_CHANGE_PUMP_PWR() (g_uiflags.bits.g_changePumpPwr = 0)
#define UI_IS_CHANGE_PUMP_PWR() (g_uiflags.bits.g_changePumpPwr)

#define UI_SET_GO2NEW_MODEL() (g_uiflags.bits.g_go2newModel = 1)
#define UI_CLEAR_GO2NEW_MODEL() (g_uiflags.bits.g_go2newModel = 0)
#define UI_IS_GO2NEW_MODEL() (g_uiflags.bits.g_go2newModel)

#define UI_SET_UNLOAD_MODEL_SETTINGS() (g_uiflags.bits.g_unloadModelSettings = 1)
#define UI_CLEAR_UNLOAD_MODEL_SETTINGS() (g_uiflags.bits.g_unloadModelSettings = 0)
#define UI_IS_UNLOAD_MODEL_SETTINGS() (g_uiflags.bits.g_unloadModelSettings)

#define UI_SET_MODEL_BUTTON_LONG_PRESSED() (g_uiflags.bits.bModelButtonLongPressed = 1)
#define UI_CLEAR_MODEL_BUTTON_LONG_PRESSED() (g_uiflags.bits.bModelButtonLongPressed = 0)
#define UI_IS_MODEL_BUTTON_LONG_PRESSED() (g_uiflags.bits.bModelButtonLongPressed)

#define UI_SET_VIEW_CALIB_VOLT() (g_uiflags.bits.g_viewCalibVolt = 1)
#define UI_CLEAR_VIEW_CALIB_VOLT() (g_uiflags.bits.g_viewCalibVolt = 0)
#define UI_IS_VIEW_CALIB_VOLT() (g_uiflags.bits.g_viewCalibVolt)

// Function prototypes
void deleteModel(lv_obj_t *obj);
// New function prototypes for setup improvements
bool initializeFileSystem();
void handleControllerError();
void navigateToInitialScreen();
