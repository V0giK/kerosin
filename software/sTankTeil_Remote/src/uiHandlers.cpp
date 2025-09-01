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

#include "uiHandlers.h"
#include "main.h"
#include "helper.h"
#include "modelManager.h"
#include "configManager.h"
#include "uartCommunication.h"
#include "ui/vars.h"
#include "ui/screens.h"
#include "ui/actions.h"
#include "ui/styles.h"
#include "ui/ui.h"
#include <esp_task_wdt.h>

// Externe Variablen aus main.cpp
extern lv_obj_t *objModelPlus;
extern lv_obj_t *objLoadedModel;
extern bool bSaveOnUnload;
extern Config config;
extern ModelParameters model;
extern UartCommunication uartCom;
extern const char *configFilePath;
extern const bool DEBUG;

// Globale Flags
extern volatile bool g_go2home;
extern volatile bool g_go2settings;
extern volatile bool g_go2settingsSystem;
extern volatile bool g_go2settingsCalibrate;
extern volatile bool g_go2model;
extern lv_event_t g_go2modelE;
extern volatile bool g_go2newModel;
extern volatile bool g_go2settingsModels;
extern lv_event_t g_go2settingsModelsE;
extern volatile bool g_unloadModelSettings;
extern volatile bool g_viewCalibVolt;
extern volatile bool g_modelLoadPending;
extern int g_modelLoadId;

ScreensEnum scrCurScreen = SCREEN_ID_MAIN;
ScreensEnum scrPrevScreen = SCREEN_ID_MAIN;

extern "C" {
    void loadScreen(ScreensEnum screen);
}

/** Handle screen flags */
void handleScreenFlags() {
  if (get_var_b_is_pumping())
    return;

  // Reset watchdog at the beginning of potentially long operations
  esp_task_wdt_reset();

  if (g_go2home) {
    g_go2home = false;
    set_var_s_screen_titel("Modellauswahl");
    if(lv_obj_is_visible(objects.cont_sys_settings)) {
      set_var_b_hide_dialog_save_system(false);
    } else if(lv_obj_is_visible(objects.cont_model_settings)) {
      set_var_b_hide_dialog_save_model(false);
    } else {
      go2screen(SCREEN_ID_MODEL_SELECT);
    }
    esp_task_wdt_reset(); // Reset after screen transition
  }
  if (g_go2settings) {
    g_go2settings = false;
    set_var_s_screen_titel("Settings/Calibration");
    //go2screen(SCREEN_ID_SETTINGS);
    set_var_s_firmware_version_remote(SW_VERSION);
    uartCom.sendData('R', COM_ID_FIRMWARE_VERSION, SW_VERSION);
    go2screen(SCREEN_ID_SETTINGS_SYSTEM);
    esp_task_wdt_reset();
  }
  if (g_go2settingsSystem) {
    g_go2settingsSystem = false;
    set_var_s_screen_titel("System Settings");
    set_var_s_firmware_version_remote(SW_VERSION);
    uartCom.sendData('R', COM_ID_FIRMWARE_VERSION, SW_VERSION);
    go2screen(SCREEN_ID_SETTINGS_SYSTEM);
    esp_task_wdt_reset();
  }
  if (g_go2settingsCalibrate) {
    g_go2settingsCalibrate = false;
    set_var_s_screen_titel("Kalibrierung Flowsensor");
    set_var_b_hide_cont_flow_calibrate(false);
    set_var_i_calib_flow_sensor(get_var_s_flow_ticks());
    
    // Reset WDT before UART communication
    esp_task_wdt_reset();
    uartCom.sendData('W', COM_ID_PUMP_MODE, int2char(MODE_CALIB_FLOW), true);
    bSaveOnUnload = false;
    go2screen(SCREEN_ID_PUMP);
    esp_task_wdt_reset();
  }
  if (g_go2model) {
    g_go2model = false;
    int id = (int)lv_event_get_user_data(&g_go2modelE);
    if(id == 0) {
      id = config.lastModel;
      if(id == 0) {
        g_go2home = true;
        return;
      }
    }

    set_var_b_hide_wait(false); // Warte-Kringel an
    lv_timer_handler();
    ui_tick();
    delay(50);

    // Save model ID and set pending flag for next loop
    g_modelLoadId = id;
    g_modelLoadPending = true;
    return; // Exit now, do NOT continue with model loading yet
  }

  // New: handle model loading in next loop
  if (g_modelLoadPending) {
    g_modelLoadPending = false;
    int id = g_modelLoadId;

    set_var_s_screen_titel("Tanken (automatik)");
    set_var_b_hide_model_fuel(false);
    set_var_b_pump_pwr_disabled(true);
    
    esp_task_wdt_reset();

    // NEU: Modell laden über modelManager
    uint32_t startTime = millis();
    bool loadSuccess = loadModel(id, model);
    if (!loadSuccess && (millis() - startTime > 3000)) {
      if(DEBUG) Serial.println("Model loading timeout!");
      set_var_s_status("Modell-Ladefehler");
      set_var_b_hide_wait(true);
      esp_task_wdt_reset();
      return;
    }
    
    esp_task_wdt_reset(); // Reset after model loading
    
    // UART communications with WDT resets between operations
    uartCom.sendData('W', COM_ID_PUMP_MODE, int2char(MODE_AUTO), true, 5);
    esp_task_wdt_reset();
    
    uartCom.sendData('W', COM_ID_BROADCAST, "1", true);
    esp_task_wdt_reset();

    // letztes geladene Modell merken - falls aktiviert
    if(config.lastModel > 0 && config.lastModel != id) {
      config.lastModel = id;
      saveConfig(config, configFilePath);
      esp_task_wdt_reset(); // Reset after file operation
    }

    // UI updates
    set_var_i_fuel_ml(0);
    set_var_s_rest_tank_time("--- s");
    lv_scale_set_range(objects.scale_ml, 0, model.getMaxRefuelMl());
    lv_bar_set_range(objects.bar_ml, 0, model.getMaxRefuelMl());
    set_var_s_cut_off_ml(int2char(model.getMaxRefuelMl(), LBL_POSTFIX_ML));
    set_var_s_status("bereit");
    
    // Reset WDT before sending model data
    esp_task_wdt_reset();
    
    // Send model data with timeout protection
    startTime = millis();
    bool sendSuccess = sendModelDataToController(model);
    if (!sendSuccess && (millis() - startTime > 5000)) { // 5 second timeout
      if(DEBUG) Serial.println("Model data send timeout!");
      set_var_s_status("Kommunikationsfehler");
      set_var_b_hide_wait(true);
      esp_task_wdt_reset();
      return;
    }
    
    go2screen(SCREEN_ID_PUMP);
    set_var_b_hide_wait(true); // Warte-Kringel aus
    esp_task_wdt_reset(); // Final reset
  }
  if(g_go2newModel) {
    g_go2newModel = false;
    objLoadedModel = objModelPlus; // erforderlich damit beim unload ein Objekt vorhanden ist
    esp_task_wdt_reset();
    loadModel(-1, model); // NEU: initialModel laden
    esp_task_wdt_reset();
    viewModelParameters((TankTypeEnum) get_var_i_tank_type_model());
    int id = (int)lv_obj_get_user_data(objModelPlus);
    set_var_s_modelname((String(get_var_s_modelname()) + String(id)).c_str());
    go2screen(SCREEN_ID_SETTINGS_MODEL);
    esp_task_wdt_reset();
  }
  if(g_go2settingsModels) {
    g_go2settingsModels = false;
    int id = (int)lv_event_get_user_data(&g_go2settingsModelsE);
    set_var_s_screen_titel("Modell bearbeiten");
    objLoadedModel = lv_event_get_target_obj(&g_go2settingsModelsE);
    lv_obj_set_user_data(objLoadedModel, (void*)id);

    esp_task_wdt_reset();
    loadModel(id, model); // NEU: Modell laden
    esp_task_wdt_reset();

    viewModelParameters((TankTypeEnum) get_var_i_tank_type_model());
    go2screen(SCREEN_ID_SETTINGS_MODEL);
    esp_task_wdt_reset();
  }
  if(g_unloadModelSettings) {
    g_unloadModelSettings = false;

    set_var_b_hide_keyboard(true);
    set_var_b_hide_numpad(true);

    if(!bSaveOnUnload) {
      bSaveOnUnload = true;
    }
    esp_task_wdt_reset(); // Final reset
  }
  
  // Final watchdog reset at the end
  esp_task_wdt_reset();
}

/** Go to screen */
void go2screen(ScreensEnum newScreen){
  scrPrevScreen = scrCurScreen;
  scrCurScreen = newScreen;
  loadScreen(scrCurScreen);

  // wann Disable - Settings-Button
  bool bDisableSetting = (scrCurScreen != SCREEN_ID_MODEL_SELECT);  // Modellliste
  //bDisableSetting = (bDisableSetting && scrCurScreen != SCREEN_ID_SETTINGS);  // Settings-Menü
  bDisableSetting = (bDisableSetting && !(scrCurScreen == SCREEN_ID_PUMP && !get_var_b_hide_model_fuel()));  // Pumpen (auto)
  bDisableSetting = (bDisableSetting && !(scrCurScreen == SCREEN_ID_PUMP && !get_var_b_hide_manuel_fuel()));  // Pumpen (manuell)
  set_var_b_disable_btn_settings(bDisableSetting);

  // wann Disable - Home-Button
  //bool bDisableHome = !(scrCurScreen == SCREEN_ID_PUMP && !get_var_b_hide_model_fuel());
  //bDisableHome = (bDisableHome || !(scrCurScreen == SCREEN_ID_PUMP && !get_var_b_hide_manuel_fuel()));
  bool bDisableHome = (scrCurScreen == SCREEN_ID_MODEL_SELECT);  // Modellliste
  bDisableHome = (bDisableHome || (scrCurScreen == SCREEN_ID_PUMP && !get_var_b_hide_cont_flow_calibrate()));
  set_var_b_disable_btn_home(bDisableHome);

  bool bDisableMod2Controller = true;
  if(scrCurScreen == SCREEN_ID_PUMP) {
    bDisableMod2Controller = get_var_b_hide_model_fuel() && get_var_b_hide_manuel_fuel();  // wenn keinde der beiden anzgezeigt wird dann Button ausblenden
  }
  set_var_b_hide_pump_save_mod_to_controller(bDisableMod2Controller);
}

/** Go to previous screen */
void go2screenPrev() {
  go2screen(scrPrevScreen);
}

/** LVGL Setup */
void lvglSetup() {
    //tft.calibrateTouch(0, 0, tft.width(), tft.height()); // Kalibrierungsroutine
    tft.begin();
    tft.setRotation(0);
    tft.setBrightness(255);

    lv_init();
    lv_tick_set_cb((lv_tick_get_cb_t)millis);

    // Display einrichten
    lv_display_t *disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_flush_cb(disp, [](lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
        uint32_t w = lv_area_get_width(area);
        uint32_t h = lv_area_get_height(area);
        tft.startWrite();
        tft.setAddrWindow(area->x1, area->y1, w, h);
        tft.writePixels((lgfx::rgb565_t *)px_map, w * h);
        tft.endWrite();
        lv_disp_flush_ready(disp);
    });
    lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    // Touchpad einrichten
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, [](lv_indev_t *indev, lv_indev_data_t *data) {
        uint16_t x, y;
        if (tft.getTouch(&x, &y)) {
            data->state = LV_INDEV_STATE_PRESSED;
            data->point.x = x;
            data->point.y = y;
        } else {
            data->state = LV_INDEV_STATE_RELEASED;
        }
    });
}

/** Set initial values for all variables */
void setInitialValues() {
    set_var_b_hide_keyboard(true);
    set_var_b_hide_numpad(true);
    set_var_b_hide_cont_flow_calibrate(true);
    set_var_b_hide_cont_model_fuel(true);
    set_var_b_hide_cont_calib_volt(true);
    set_var_b_hide_model_fuel(true);
    set_var_b_hide_manuel_fuel(true);

    set_var_b_hide_box_start_error(true);
    set_var_b_hide_wait(true);

    set_var_b_hide_dialog_save_system(true);
    set_var_b_hide_dialog_save_model(true);

    set_var_b_pump_pwr_disabled(false);
    set_var_b_is_pumping(false);

    set_var_b_disable_btn_home(false);
    set_var_b_disable_btn_settings(false);
}

/** Show error message and enter sleep mode */
void showErrorAndSleep(const char* message) {
    set_var_s_status(message);
    set_var_b_hide_box_start_error(false);

    // Show error for 10 seconds then sleep
    uint32_t startTime = millis();
    while (millis() - startTime < 10000) {
        lv_timer_handler(); // Use only lv_timer_handler() for LVGL 9.x
        ui_tick();
        uartCom.tick();
        esp_task_wdt_reset();
        delay(50);
    }

    // Enter deep sleep or reset
    esp_restart();
}

/** Update the display */
void updateDisplay(int iterations) {
    for(int i = 0; i < iterations; i++) {
        lv_timer_handler(); // Use only lv_timer_handler() for LVGL 9.x
        ui_tick();
        delay(10); // Short delay instead of tight loop
    }
}