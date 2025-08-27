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

#include "inputHandlers.h"
#include "main.h"
#include "helper.h"
#include "uartCommunication.h"
#include "ModelParameters.h"
#include "ui/vars.h"
#include "ui/screens.h"
#include "ui/actions.h"
#include "snakeGame.h"
#include "configManager.h" // Add this include for Config type

// Externe Variablen aus main.cpp
extern lv_obj_t *objModelPlus;
extern lv_obj_t *objLoadedModel;
extern bool bSaveOnUnload;
extern Config config;
extern ModelParameters model;
extern UartCommunication uartCom;
extern const char *configFilePath;
extern const bool DEBUG;
extern SnakeGame snakeGame;

// Globale Flags
extern volatile bool g_eventButtonClick;
extern lv_event_t g_eventButtonClickE;

// Add extern declaration for go2screenPrev:
extern void go2screenPrev();

// Add extern declaration for deleteModel:
extern void deleteModel(lv_obj_t *obj);

// Add extern declaration for go2screen:
extern void go2screen(ScreensEnum screen);

// Add extern declaration for btnModelSaveClick:
extern void btnModelSaveClick(lv_obj_t *objLoadedModel, lv_obj_t *objModelPlus, ModelParameters &model);

// Add extern declaration for viewModelParameters:
extern void viewModelParameters(TankTypeEnum tankType);

// Add externs for keyboard flags and objects
extern volatile bool g_keyboardShow;
extern lv_event_t g_keyboardShowE;
extern void set_var_b_hide_keyboard(bool);

// Add externs for numpad flags and objects
extern volatile bool g_numpadShow;
extern lv_event_t g_numpadShowE;
extern void set_var_b_hide_numpad(bool);

// Add externs for settings flags and variables
extern volatile bool g_unloadSystemSettings;
extern volatile bool g_viewCalibVolt;
extern void set_var_b_hide_cont_calib_volt(bool);
extern bool bSaveOnUnload;
extern void set_var_s_akku_volt_messure(const char*);
extern const char* get_var_s_akku_volt();


/** Handle Button Click */
void handleButtonClick() {
    if (get_var_b_is_pumping())
        return;

    if(g_eventButtonClick) {
        g_eventButtonClick = false;

        lv_obj_t *obj = lv_event_get_current_target_obj(&g_eventButtonClickE);
        int userData = (int)lv_event_get_user_data(&g_eventButtonClickE);

        switch (userData) {
            case BTN_FLOW_RESET:
                set_var_i_calib_flow_sensor("0");
                uartCom.sendData('W', COM_ID_FLOWCALIBRATE, "0", true);
                break;
            case BTN_FLOW_SET:
                if(get_var_i_calib_flow_sensor()[0] != '\0' && strcmp(get_var_i_calib_flow_sensor(), "0") != 0) {
                    set_var_s_flow_ticks(get_var_i_calib_flow_sensor());
                }
                go2screenPrev();
                break;
            case BTN_MODEL_RESET_TANKCOUNT:
                model.setFuelingCount(0);
                set_var_s_fueling_count("0");
                break;
            case BTN_MODEL_RESET_TOTAL:
                model.setFuelingTotal(0);
                set_var_s_fueling_total("0");
                break;
            case BTN_MODEL_DELETE:
                if(DEBUG) Serial.println("BTN_MODEL_DELETE clicked");
                if (!objLoadedModel) {
                    if(DEBUG) Serial.println("Error: objLoadedModel is NULL");
                    break;
                }
                deleteModel(objLoadedModel);
                bSaveOnUnload = false;
                objLoadedModel = NULL;
                go2screen(SCREEN_ID_MODEL_SELECT);
                if(DEBUG) Serial.println("Navigation to model select screen completed");
                break;
            case BTN_MODEL_SAVE_YES:
                btnModelSaveClick(objLoadedModel, objModelPlus, model);
                set_var_b_hide_dialog_save_model(true);
                go2screen(SCREEN_ID_MODEL_SELECT);
                break;
            case BTN_MODEL_SAVE_NO:
                set_var_b_hide_dialog_save_model(true);
                go2screen(SCREEN_ID_MODEL_SELECT);
                break;
            case BTN_SYSTEM_SAVE_YES:
                uartCom.sendData('W', COM_ID_BEEP, (get_var_b_signal()?"1":"0"), true);
                uartCom.sendData('W', COM_ID_FLOW_TICKS, get_var_s_flow_ticks(), true);
                uartCom.sendData('W', COM_ID_PRESSURE_AVG, get_var_s_pressure_avg(), true);
                uartCom.sendData('W', COM_ID_AKKU_MIN, int2char((int)(atoff(get_var_s_akku_min_v()) * 10)), true);
                uartCom.sendData('W', COM_ID_PWR_MANUEL, get_var_s_pump_pwr_manu(), true);
                uartCom.sendData('W', COM_ID_SYSTEM_OFF, get_var_s_sys_power_off_time(), true);
                uartCom.sendData('W', COM_ID_WRITE_EEPROM, "5", true);

                config.owner = get_var_s_owner();
                if(get_var_b_load_last_model()) {
                    if(config.lastModel <= 0) {
                        config.lastModel = 1;
                    }
                } else {
                    config.lastModel = 0;
                }
                config.beep = get_var_b_signal();
                config.flowTicks = atoi(get_var_s_flow_ticks());
                config.pressureAvg = atoi(get_var_s_pressure_avg());
                config.akkuMinV = (int)(atoff(get_var_s_akku_min_v()) * 10);
                // TODO: Variablen fehlten noch
                //config.akkuFactor = ???;
                config.sysPowerOffTime = atoi(get_var_s_sys_power_off_time());
                config.pumpPwrManu = atoi(get_var_s_pump_pwr_manu());
                config.pumpPwrCalib = atoi(get_var_s_pump_pwr_calib());

                saveConfig(config, configFilePath);

                set_var_b_hide_dialog_save_system(true);
                go2screen(SCREEN_ID_MODEL_SELECT);
                break;
            case BTN_SYSTEM_SAVE_NO:
                set_var_b_hide_dialog_save_system(true);
                applyConfigToUI(config);
                go2screen(SCREEN_ID_MODEL_SELECT);
                break;
            case BTN_SYSTEM_SEND_CALIBVOLT:
                uartCom.sendData('W', COM_ID_AKKU_VOLT, int2char((int)(atoff(get_var_s_akku_volt_messure()) * 100)), true);
                set_var_b_hide_cont_calib_volt(true);
                set_var_b_hide_numpad(true);
                break;
            case BTN_MODEL_SAVE2CONTROLLER:
                uartCom.sendData('W', COM_ID_SAVE_MODEL_EEPROM, "5", true);
                break;
            case BTN_SNAKE_START:
                snakeGame.start();
                break;
            case BTN_SNAKE_STOP:
                snakeGame.stop();
                break;
            case BTN_MODEL_TYPE_CHG:
                viewModelParameters((TankTypeEnum) get_var_i_tank_type_model());
                break;
            default:
                break;
        }
    }
}

/** Handle keyboard events */
void handleKeyboard() {
    if (g_keyboardShow) {
        g_keyboardShow = false;
        lv_obj_t *obj = lv_event_get_current_target_obj(&g_keyboardShowE);
        int userData = (int)lv_event_get_user_data(&g_keyboardShowE);
        if (userData == 1) {
            // Textarea an alle Keyboards zuordnen
            lv_keyboard_set_textarea(objects.cont_keyboard_settings__kb_keyboard, obj);
            lv_keyboard_set_textarea(objects.cont_keyboard_model__kb_keyboard, obj);
            set_var_b_hide_keyboard(false);
        } else {
            set_var_b_hide_keyboard(true);
        }
    }
}

/** Numpad anzeigen/verstecken */
void handleNumpad() {
    if (g_numpadShow) {
        g_numpadShow = false;
        lv_obj_t *obj = lv_event_get_current_target_obj(&g_numpadShowE);
        int userData = (int)lv_event_get_user_data(&g_numpadShowE);
        if (userData == 1) {
            // Textarea an alle Numpads zuordnen
            lv_keyboard_set_textarea(objects.cont_numpad_settings__kb_numpad, obj);
            lv_keyboard_set_textarea(objects.cont_numpad_model__kb_numpad, obj);
            set_var_b_hide_numpad(false);
        } else {
            set_var_b_hide_numpad(true);
        }
    }
}

/** Einstellungen Seite behandeln */
void handleSettingsPage() {
    if(g_unloadSystemSettings) {
        g_unloadSystemSettings = false;

        set_var_b_hide_keyboard(true);
        set_var_b_hide_numpad(true);
        set_var_b_hide_cont_calib_volt(true);

        if(!bSaveOnUnload ) {
            bSaveOnUnload = true;
        }
    }
    if(g_viewCalibVolt) {
        g_viewCalibVolt = false;
        set_var_s_akku_volt_messure((const char*)get_var_s_akku_volt());
        set_var_b_hide_cont_calib_volt(false);
        set_var_b_hide_numpad(false);
    }
}
