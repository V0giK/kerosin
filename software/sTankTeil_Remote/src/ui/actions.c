// Path ..src/ui/action.c
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


#include "actions.h"

// Union für UI-Flags definiert in main.cpp
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

// Volatile bools entfernt, nun in union
lv_event_t g_keyboardShowE = {};
lv_event_t g_numpadShowE = {};
lv_event_t g_go2settingsModelsE = {};
lv_event_t g_resetDataFieldsE = {};
lv_event_t g_go2modelE = {};
lv_event_t g_go2calibrateModelE = {};
lv_event_t g_eventButtonClickE = {};
lv_event_t g_changePumpPwrE = {};
lv_event_t g_go2newModelE = {};



void action_go2home(lv_event_t *e) {
    g_uiflags.bits.g_go2home = true;
}

void action_go2settings(lv_event_t *e) {
    g_uiflags.bits.g_go2settings = true;
}


void action_keyboard_show(lv_event_t *e) {
    g_uiflags.bits.g_keyboardShow = true;
    g_keyboardShowE = *e;
}

void action_numpad_show(lv_event_t *e) {
   g_uiflags.bits.g_numpadShow = true;
    g_numpadShowE = *e;
}

void action_go2settings_system(lv_event_t *e) {
    g_uiflags.bits.g_go2settingsSystem = true;
}

void action_go2settings_calibrate(lv_event_t *e) {
    g_uiflags.bits.g_go2settingsCalibrate = true;
}

void action_go2settings_models(lv_event_t *e) {
    g_uiflags.bits.g_go2settingsModels = true;
    g_go2settingsModelsE = *e;

    g_uiflags.bits.bModelButtonLongPressed = true;
}

void action_unload_manual_fuel(lv_event_t *e) {
    g_uiflags.bits.g_unloadManuelFuel = true;
}

void action_event_button_click(lv_event_t *e) {
    g_uiflags.bits.g_eventButtonClick = true;
    g_eventButtonClickE = *e;
}

void action_pump_stop(lv_event_t *e) {
    g_uiflags.bits.g_pumpStop = true;
}

void action_pump_in(lv_event_t *e) {
    g_uiflags.bits.g_pumpIn = true;
}

void action_pump_out(lv_event_t *e) {
    g_uiflags.bits.g_pumpOut = true;
}

void action_go2model(lv_event_t *e) {
    if(g_uiflags.bits.bModelButtonLongPressed) {
        g_uiflags.bits.bModelButtonLongPressed = false;
    } else {
        g_uiflags.bits.g_go2model = true;
        g_go2modelE = *e;
    }
}

void action_go2_manuel_pump(lv_event_t *e) {
    g_uiflags.bits.g_go2manuelPump = true;
}

void action_unload_system_settings(lv_event_t *e) {
    g_uiflags.bits.g_unloadSystemSettings = true;
}

void action_change_pump_pwr(lv_event_t *e) {
    g_uiflags.bits.g_changePumpPwr = true;
    g_changePumpPwrE = *e;
}

void action_go2new_model(lv_event_t *e) {
    g_uiflags.bits.g_go2newModel = true;
    g_go2newModelE = *e;
}

void action_unload_model_settings(lv_event_t *e) {
    g_uiflags.bits.g_unloadModelSettings = true;
}

void action_view_calib_volt(lv_event_t *e) {
    g_uiflags.bits.g_viewCalibVolt = true;
}