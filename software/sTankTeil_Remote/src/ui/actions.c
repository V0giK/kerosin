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

volatile bool g_go2home = false;
volatile bool g_go2settings = false;
volatile bool g_keyboardShow = false;
lv_event_t g_keyboardShowE = {};
volatile bool g_numpadShow = false;
lv_event_t g_numpadShowE = {};
volatile bool g_go2settingsSystem = false;
volatile bool g_go2settingsCalibrate = false;
volatile bool g_go2settingsModels = false;
lv_event_t g_go2settingsModelsE = {};
volatile bool g_unloadManuelFuel = false;
lv_event_t g_resetDataFieldsE = {};
volatile bool g_pumpStop;
volatile bool g_pumpIn;
volatile bool g_pumpOut;
volatile bool g_go2model = false;
lv_event_t g_go2modelE = {};
volatile bool g_go2manuelPump = false;
volatile bool g_go2calibrateModel = false;
lv_event_t g_go2calibrateModelE = {};
volatile bool g_unloadSystemSettings = false;
volatile bool g_eventButtonClick = false;
lv_event_t g_eventButtonClickE = {};
volatile bool g_changePumpPwr = false;
lv_event_t g_changePumpPwrE = {};
volatile bool g_go2newModel = false;
lv_event_t g_go2newModelE = {};
volatile bool g_unloadModelSettings = false;

volatile bool bModelButtonLongPressed = false;


void action_go2home(lv_event_t *e) {
    g_go2home = true;
}

void action_go2settings(lv_event_t *e) {
    g_go2settings = true;
}


void action_keyboard_show(lv_event_t *e) {
    g_keyboardShow = true;
    g_keyboardShowE = *e;
}

void action_numpad_show(lv_event_t *e) {
   g_numpadShow = true;
   g_numpadShowE = *e;
}

void action_go2settings_system(lv_event_t *e) {
    g_go2settingsSystem = true;
}

void action_go2settings_calibrate(lv_event_t *e) {
    g_go2settingsCalibrate = true;
}

void action_go2settings_models(lv_event_t *e) {
    g_go2settingsModels = true;
    g_go2settingsModelsE = *e;

    bModelButtonLongPressed = true;
}

void action_unload_manual_fuel(lv_event_t *e) {
    g_unloadManuelFuel = true;
}

void action_event_button_click(lv_event_t *e) {
    g_eventButtonClick = true;
    g_eventButtonClickE = *e;
}

void action_pump_stop(lv_event_t *e) {
    g_pumpStop = true;
}

void action_pump_in(lv_event_t *e) {
    g_pumpIn = true;
}
void action_pump_out(lv_event_t *e) {
    g_pumpOut = true;
}

void action_go2model(lv_event_t *e) {
    if(bModelButtonLongPressed) {
        bModelButtonLongPressed = false;
    } else {
        g_go2model = true;
        g_go2modelE = *e;
    }
}

void action_go2_manuel_pump(lv_event_t *e) {
    g_go2manuelPump = true;
}

void action_unload_system_settings(lv_event_t *e) {
    g_unloadSystemSettings = true;
}

void action_change_pump_pwr(lv_event_t *e) {
    g_changePumpPwr = true;
    g_changePumpPwrE = *e;
}

void action_go2new_model(lv_event_t *e) {
    g_go2newModel = true;
    g_go2newModelE = *e;
}

void action_unload_model_settings(lv_event_t *e) {
   g_unloadModelSettings = true;
}
