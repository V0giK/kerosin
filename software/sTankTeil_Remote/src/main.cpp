#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>  // Für den Dateizugriff
#include "main.h"
#include <lvgl.h>
#include "lovyanGfxSetup.h"
#include "ui/ui.h"
#include "ui/vars.h"
#include "ui/styles.h"
#include "ui/actions.h"
#include "helper.h"
#include "uartCommunication.h"
#include "ModelParameters.h"
#include "snakeGame.h"
//#include "tetris.h"


// Debug-Ausgabe steuern
const bool DEBUG = false;
//LV_LOG_USER("dfg");

// LVGL Display-Konfiguration
#define TFT_HOR_RES SCREEN_WIDTH
#define TFT_VER_RES SCREEN_HEIGHT
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

LGFX tft;

// Globale Flags (aus actions.c)
extern volatile bool g_go2home;
extern volatile bool g_go2settings;
extern volatile bool g_keyboardShow;
extern lv_event_t g_keyboardShowE;
extern volatile bool g_numpadShow;
extern lv_event_t g_numpadShowE;
extern volatile bool g_go2settingsSystem;
extern volatile bool g_go2settingsCalibrate;
extern volatile bool g_go2settingsModels;
extern lv_event_t g_go2settingsModelsE;
extern volatile bool g_unloadManuelFuel;
extern lv_event_t g_resetDataFieldsE;
extern volatile bool g_pumpStop;
extern volatile bool g_pumpIn;
extern volatile bool g_pumpOut;
extern volatile bool g_go2model;
extern lv_event_t g_go2modelE;
extern volatile bool g_go2manuelPump;
extern volatile bool g_go2calibrateModel;
extern lv_event_t g_go2calibrateModelE;
extern volatile bool g_unloadSystemSettings;
extern volatile bool g_eventButtonClick;
extern lv_event_t g_eventButtonClickE;
extern volatile bool g_changePumpPwr;
extern lv_event_t g_changePumpPwrE;
extern volatile bool g_go2newModel;
extern lv_event_t g_go2newModelE;
extern volatile bool g_unloadModelSettings;
extern volatile bool bModelButtonLongPressed;

// namespace GlobalFlags {
//   bool g_go2home, g_go2settings, g_go2settingsSystem, g_go2settingsCalibrate, g_go2manuelPump;
//   bool g_pumpStop, g_pumpIn, g_pumpOut, g_go2model, g_unloadSystemSettings;
//   bool g_keyboardShow, g_numpadShow, g_unloadManuelFuel;
//   bool g_eventButtonClick, g_changePumpPwr, g_go2newModel, g_unloadModelSettings;
//   bool g_go2settingsModels;
//   lv_event_t g_keyboardShowE, g_numpadShowE, g_go2modelE, g_eventButtonClickE;
//   lv_event_t g_changePumpPwrE, g_go2newModelE, g_go2settingsModelsE;
// }
// using namespace GlobalFlags;

lv_obj_t *objModelPlus = NULL;
lv_obj_t *objLoadedModel = NULL;

bool bSaveOnUnload = true; // Werte beim verlassen der Page speichern

// Pfad zur JSON-Konfigurationsdatei
const char *configFilePath = "/config.json";

// Globale Konfigurationsdaten
struct Config {
    String owner;
    int lastModel;
    int beep;
    int flowTicks;
    int pressureAvg;
    int akkuMinV;
    int akkuFactor;
    int sysPowerOffTime;
    int pumpPwrManu;
    int pumpPwrCalib;
} config;

// Modell Daten
ModelParameters model;

// UART-Kommunikation
UartCommunication uartCom(17, 18, DEBUG, 2); // RX = GPIO19, TX = GPIO20

// Snake
SnakeGame snakeGame;
// Tetris
//Tetris tetrisGame;

// Funktionsprototypen
void lvglSetup();
void setInitialValues();
bool searchAndLoadController();
void handleScreenFlags();
void handlePumpControl();
void handleButtonClick();
void handleSettingsPage();
void handleKeyboard();
void handleNumpad();
void handleManualPump();
void go2screen(ScreensEnum);
bool requestAndValidateConfiguration(int16_t, const String &, unsigned long, unsigned long);
bool loadConfig();
bool saveConfig();
lv_obj_t* addModelButton2container(String, int);
lv_obj_t* addModelPlusButton2container(int);
bool loadModel(int);
bool saveModel(const String &);
void deleteModel(lv_obj_t *);
void btnModelSaveClick();
bool sendModelDataToController();
void viewModelParameters(TankTypeEnum tankType);

ScreensEnum scrCurScreen = SCREEN_ID_MAIN;
ScreensEnum scrPrevScreen = SCREEN_ID_MAIN;




void printSystemSettings() {
  if(DEBUG) {
    // Load Config in Variables
    Serial.println("Owner:                " + String(get_var_s_owner()));
    Serial.println("Load last model:      " + String(get_var_b_load_last_model()));
    Serial.println("Signalton:            " + String(get_var_b_signal()));
    Serial.println("Flow Ticks:           " + String(get_var_s_flow_ticks()));
    Serial.println("Flow Messungen AVG:   " + String(get_var_s_pressure_avg()));
    Serial.println("Akku min. V:          " + String(get_var_s_akku_min_v()));
    // Variable für config.akkuFactor fehlt noch!
    Serial.println("System idle off:      " + String(get_var_s_sys_power_off_time()));
    Serial.println("PWR manuell:          " + String(get_var_s_pump_pwr_manu()));
    Serial.println("PWR Calib:            " + String(get_var_s_pump_pwr_calib()));
    Serial.println();
  }
}

// LVGL Display und Touchpad Setup
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


void setup() {

    Serial.begin(115200);
    uartCom.begin(9600); // 19200

    // Display und Touch-Setup
    lvglSetup();
    ui_init();

    // Initialwerte setzen
    setInitialValues();

    // Initialisiere LittleFS
    if (!LittleFS.begin()) {
        if(DEBUG) Serial.println("LittleFS konnte nicht initialisiert werden!");
        return;
    }

    // Display aktualisieren
    for(uint8_t i=0; i <=5; i++) {
      lv_task_handler();
      ui_tick();
    }

    // Konfiguration laden
    if (!loadConfig()) {
        // Falls Datei nicht existiert, mit Standardwerten speichern
        config = {
          .owner = "I'm the BOSS",
          .lastModel = 0,
          .beep = 1,
          .flowTicks = 5315,
          .pressureAvg = 6,
          .akkuMinV = 105,
          .akkuFactor = 1,
          .sysPowerOffTime = 180,
          .pumpPwrManu = 80,
          .pumpPwrCalib = 80
        };
        saveConfig();
    }

    bool bContollerIsVisible = searchAndLoadController(); // Suche nach dem Controller


    // Load Config in UI-Variables
    set_var_s_owner(config.owner.c_str());
    set_var_b_load_last_model(!(config.lastModel == 0));
    set_var_b_signal((config.beep == 1));
    set_var_s_flow_ticks(int2char(config.flowTicks));
    set_var_s_pressure_avg(int2char(config.pressureAvg));
    set_var_s_akku_min_v(String(config.akkuMinV / 10.0).c_str());
    // TODO:
    // Variable für config.akkuFactor fehlt noch!
    set_var_s_sys_power_off_time(int2char(config.sysPowerOffTime));
    set_var_s_pump_pwr_manu(int2char(config.pumpPwrManu));
    set_var_s_pump_pwr_calib(int2char(config.pumpPwrCalib));
    set_var_i_pump_pwr(config.pumpPwrCalib);
    set_var_s_pump_pwr(int2char(get_var_i_pump_pwr(), LBL_POSTFIX_PROZENT));

    if(DEBUG) printSystemSettings();



    if(!bContollerIsVisible) {
      set_var_b_hide_box_start_error(false);
      while(true){
        lv_task_handler();
        ui_tick();
        uartCom.tick();
      }
    }

    // // Display aktualisieren
    // for(uint8_t i=0; i <=5; i++) {
    //   lv_task_handler();
    //   ui_tick();
    // }

    if(DEBUG) Serial.println("loading models...");
    set_var_s_status("loading models...");

    // for(uint16_t c=500;c > 0; c--){
    //   lv_task_handler();
    //   ui_tick();
    //   delay(1);
    // }

    // Modelle aus /models/ laden
    File root = LittleFS.open("/models/");
    File file = root.openNextFile();
    int maxId = 1;
    int curId;
    while (file) {
        String fileName = file.name();
        curId = fileName.toInt();
        if(curId > maxId) maxId = curId;

        StaticJsonDocument<128> doc;
        deserializeJson(doc, file);
        addModelButton2container(doc["modelName"].as<const char*>(), curId);

        if(DEBUG) Serial.println("File: " + String(fileName.toInt()) + ", Modell: " + doc["modelName"].as<String>());
        file.close();

        file = root.openNextFile();
    }
    objModelPlus = addModelPlusButton2container(++maxId);



    for(uint16_t c=2000;c > 0; c--){
      lv_task_handler();
      ui_tick();
      delay(1);
    }

    set_var_s_status("");

    // letztes Modell laden
    if(config.lastModel > 0) {
      g_go2model = true;
    } else {
      g_go2home = true;
    }
}

// Initialwerte setzen
void setInitialValues() {
    set_var_b_hide_keyboard(true);
    set_var_b_hide_numpad(true);
    set_var_b_hide_cont_flow_calibrate(true);
    set_var_b_hide_cont_model_fuel(true);
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

// Suche Controller
bool searchAndLoadController() {
    // Warte auf Konfigurationsdaten
    bool bConfOk = true;
    const unsigned long globalTimeout = 10000; // Gesamttimeout für alle Anfragen (10 Sekunden)
    unsigned long startTime = millis();

    set_var_s_status("search controller...");
    if(DEBUG) Serial.println("search controller...");

    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_TYPE, "Geräte Typ", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_FLOW_TICKS, "Flow Ticks", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_PRESSURE_AVG, "Pressure Average", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_AKKU_MIN, "Minimalspannung", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_AKKU_CALIB, "Akku Kalibrierungsfaktor", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_BEEP, "Signalton", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_PWR_MANUEL, "Pumpleistung manuell", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_SYSTEM_OFF, "System idle Abschaltzeit", startTime, globalTimeout);

    if(!bConfOk) {
      if(DEBUG) Serial.println("!!!! lesen vom Controller nicht möglich !!!!");
    }

    return bConfOk;
    
}

// Hauptprogramm Loop
void loop() {
    lv_task_handler();
    ui_tick();
    uartCom.tick(); // Empfangene Daten verarbeiten

    handleScreenFlags();
    handlePumpControl();
    handleManualPump();
    handleButtonClick();
    handleSettingsPage();
    handleKeyboard();
    handleNumpad();
}

// Bildschirm-Flags verarbeiten
void handleScreenFlags() {
  if (get_var_b_is_pumping())
    return;

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
  }
  if (g_go2settings) {
    g_go2settings = false;
    set_var_s_screen_titel("Settings/Calibration");
    go2screen(SCREEN_ID_SETTINGS);
  }
  if (g_go2settingsSystem) {
    g_go2settingsSystem = false;
    set_var_s_screen_titel("System Settings");
    go2screen(SCREEN_ID_SETTINGS_SYSTEM);
  }
  if (g_go2settingsCalibrate) {
    g_go2settingsCalibrate = false;
    set_var_s_screen_titel("Kalibrierung Flowsensor");
    set_var_b_hide_cont_flow_calibrate(false);
    set_var_i_calib_flow_sensor(get_var_s_flow_ticks());
    uartCom.sendData('W', COM_ID_PUMP_MODE, int2char(MODE_CALIB_FLOW), true);
    bSaveOnUnload = false;
    go2screen(SCREEN_ID_PUMP);
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

    set_var_s_screen_titel("Tanken (automatik)");
    set_var_b_hide_model_fuel(false);
    set_var_b_pump_pwr_disabled(true);
    loadModel(id);
    uartCom.sendData('W', COM_ID_PUMP_MODE, int2char(MODE_AUTO), true, 5);
    uartCom.sendData('W', COM_ID_BROADCAST, "1", true);

    // letztes geladene Modell merken - falls aktiviert
    if(config.lastModel > 0 && config.lastModel != id) {
      config.lastModel = id;
      saveConfig();
    }

    // UI
    set_var_i_fuel_ml(0);
    lv_scale_set_range(objects.scale_ml, 0, model.getMaxRefuelMl());
    lv_bar_set_range(objects.bar_ml, 0, model.getMaxRefuelMl());
    set_var_s_cut_off_ml(int2char(model.getMaxRefuelMl(), LBL_POSTFIX_ML));
    set_var_s_status("bereit");

    sendModelDataToController();
    go2screen(SCREEN_ID_PUMP);

    set_var_b_hide_wait(true); // Warte-Kringel aus
  }
  if(g_go2newModel) {
    g_go2newModel = false;
    // int id = (int)lv_event_get_user_data(&g_go2newModelE);
    // set_var_s_screen_titel("Modell erstellen");
    // objLoadedModel = lv_event_get_target_obj(&g_go2newModelE);
    // lv_obj_set_user_data(objLoadedModel, (void*)id);
    objLoadedModel = objModelPlus;  // erforderlich damit beim unload ein Objekt vorhanden ist
    loadModel(-1);
    viewModelParameters((TankTypeEnum) get_var_i_tank_type_model());
    int id = (int)lv_obj_get_user_data(objModelPlus);
    set_var_s_modelname((String(get_var_s_modelname()) + String(id)).c_str());
    go2screen(SCREEN_ID_SETTINGS_MODEL);
  }
  if(g_go2settingsModels) {
    g_go2settingsModels = false;
    int id = (int)lv_event_get_user_data(&g_go2settingsModelsE);
    set_var_s_screen_titel("Modell bearbeiten");
    objLoadedModel = lv_event_get_target_obj(&g_go2settingsModelsE);
    lv_obj_set_user_data(objLoadedModel, (void*)id);
    loadModel(id);
    viewModelParameters((TankTypeEnum) get_var_i_tank_type_model());
    go2screen(SCREEN_ID_SETTINGS_MODEL);
  }
  if(g_unloadModelSettings) {
    g_unloadModelSettings = false;

    set_var_b_hide_keyboard(true);
    set_var_b_hide_numpad(true);

    if(!bSaveOnUnload) {
      bSaveOnUnload = true;
    }
  }
}

// Pumpensteuerung verarbeiten
void handlePumpControl() {
    if (g_pumpIn) {
        g_pumpIn = false;

        if(get_var_b_is_pumping()) {
          g_pumpStop = true;
        } else {
          set_var_b_is_pumping(true);
          set_var_b_disable_btn_home(true);
          set_var_b_disable_btn_settings(true);
          uartCom.sendData('W', COM_ID_PUMP_PWR, String(get_var_i_pump_pwr()), true);
          uartCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_TANKEN), true);
        }
    }
    if (g_pumpOut) {
        g_pumpOut = false;

        if(get_var_b_is_pumping()) {
          g_pumpStop = true;
        } else {
          set_var_b_is_pumping(true);
          set_var_b_disable_btn_home(true);
          set_var_b_disable_btn_settings(true);
          uartCom.sendData('W', COM_ID_PUMP_PWR, String(get_var_i_pump_pwr()), true);
          uartCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_ENTTANKEN), true);
        }
    }
    if (g_pumpStop) {
        g_pumpStop = false;
        set_var_b_is_pumping(false);
        // TODO: darauf achten ob bei jeglichem Stop die Buttons wieder aktiviert werden dürfen
        set_var_b_disable_btn_home(false);
        set_var_b_disable_btn_settings(false);
        uartCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP), true);
    }
    if (g_changePumpPwr) {
        g_changePumpPwr = false;
        set_var_s_pump_pwr(int2char(get_var_i_pump_pwr(), LBL_POSTFIX_PROZENT));
        uartCom.sendData('W', COM_ID_PUMP_PWR, String(get_var_i_pump_pwr()), true);
    }
}

// Keyboard anzeigen/verstecken
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

// Numpad anzeigen/verstecken
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

// Handler für manuelles Pumpen
void handleManualPump() {
    if (g_go2manuelPump) {  // Prüfen, ob das Flag gesetzt ist
        g_go2manuelPump = false; // Flag zurücksetzen

        // Titel für den manuellen Pump-Bildschirm setzen
        set_var_s_screen_titel("Tanken (manuell)");

        // Manuelle Pumpleistung
        set_var_i_pump_pwr(atoi(get_var_s_pump_pwr_manu()));
        set_var_s_pump_pwr(int2char(get_var_i_pump_pwr()));

        set_var_s_fuel_ml("0 ml");

        // UI-Elemente anpassen oder anzeigen
        // TODO : !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        set_var_b_hide_manuel_fuel(false); // Zeige manuelles Tanken-UI an
        uartCom.sendData('W', COM_ID_PUMP_MODE, int2char(MODE_MANUELL), true);
        uartCom.sendData('W', COM_ID_BROADCAST, "1", true);

        // Bildschirm wechseln
        go2screen(SCREEN_ID_PUMP);

        if(DEBUG) Serial.println("Manuelles Pumpen gestartet."); // Debug-Ausgabe
    }

    if (g_unloadManuelFuel) { // Prüfen, ob das Flag gesetzt ist
        g_unloadManuelFuel = false; // Flag zurücksetzen

        // UI-Elemente ausblenden oder anpassen
        set_var_b_hide_cont_model_fuel(true);
        set_var_b_hide_cont_flow_calibrate(true);
        set_var_b_hide_model_fuel(true);
        set_var_b_hide_manuel_fuel(true);

        set_var_b_pump_pwr_disabled(false);

        uartCom.sendData('W', COM_ID_BROADCAST, "0", true);
    }
}

// Handler für Button Clicks
void handleButtonClick() {
  if (get_var_b_is_pumping())
    return;

  if(g_eventButtonClick) {
    g_eventButtonClick = false;

    lv_obj_t *obj = lv_event_get_current_target_obj(&g_eventButtonClickE);
    int userData = (int)lv_event_get_user_data(&g_eventButtonClickE);

    switch (userData) {
      case BTN_FLOW_RESET:       // Reset - SettingsCalibrate FlowTicks
        set_var_i_calib_flow_sensor("0");
        uartCom.sendData('W', COM_ID_FLOWCALIBRATE, "0", true);
        break;

      case BTN_FLOW_SET:        // Set Flow - SettingsCalibrate FlowTicks übernehmen
        if(get_var_i_calib_flow_sensor()[0] != '\0' && strcmp(get_var_i_calib_flow_sensor(), "0") != 0) {
          set_var_s_flow_ticks(get_var_i_calib_flow_sensor());
        }
        go2screen(scrPrevScreen);
        break;

      case BTN_MODEL_RESET_TANKCOUNT: // Reset - Anzahl Tankungen
        model.setFuelingCount(0);
        set_var_s_fueling_count("0");
        break;

      case BTN_MODEL_RESET_TOTAL: // Reset - Gesamttankmenge
        model.setFuelingTotal(0);
        set_var_s_fueling_total("0");
        break;

      case BTN_MODEL_DELETE: // Modell löschen
        // Modell löschen
        deleteModel(objLoadedModel);
        bSaveOnUnload = false;
        go2screen(scrPrevScreen);
        break;

      case BTN_MODEL_SAVE_YES: // Modell speichern
        btnModelSaveClick();

        // denke es ist nichts zu machen, da ja wieder ein neues Modell ausgewählt werden kann
        set_var_b_hide_dialog_save_model(true);
        go2screen(SCREEN_ID_MODEL_SELECT);
        break;

      case BTN_MODEL_SAVE_NO: // Modell nicht speichern
        // denke es ist nichts zu machen, da ja wieder ein neues Modell ausgewählt werden kann
        set_var_b_hide_dialog_save_model(true);
        go2screen(SCREEN_ID_MODEL_SELECT);
        break;

      case BTN_SYSTEM_SAVE_YES: // Systemeinstellungen speichern
        uartCom.sendData('W', COM_ID_BEEP, (get_var_b_signal()?"1":"0"), true);
        uartCom.sendData('W', COM_ID_FLOW_TICKS, get_var_s_flow_ticks(), true);
        uartCom.sendData('W', COM_ID_PRESSURE_AVG, get_var_s_pressure_avg(), true);
        uartCom.sendData('W', COM_ID_AKKU_MIN, int2char((int)(atoff(get_var_s_akku_min_v()) * 10)), true);
        uartCom.sendData('W', COM_ID_PWR_MANUEL, get_var_s_pump_pwr_manu(), true);
        uartCom.sendData('W', COM_ID_SYSTEM_OFF, get_var_s_sys_power_off_time(), true);
        uartCom.sendData('W', COM_ID_WRITE_EEPROM, "5", true);

        // config.json
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

        saveConfig();

        set_var_b_hide_dialog_save_system(true);
        go2screen(SCREEN_ID_MODEL_SELECT);
        break;

      case BTN_SYSTEM_SAVE_NO: // Systemeinstellungen nicht speichern
        // denke es ist nichts zu machen
        set_var_b_hide_dialog_save_system(true);
        go2screen(SCREEN_ID_MODEL_SELECT);
        break;

      case BTN_MODEL_SAVE2CONTROLLER:
        uartCom.sendData('W', COM_ID_SAVE_MODEL_EEPROM, "5", true);
        break;

      case BTN_SNAKE_START:
        snakeGame.start();
        //tetrisGame.startGame();
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

void handleSettingsPage() {
  if(g_unloadSystemSettings) {
    g_unloadSystemSettings = false;

    set_var_b_hide_keyboard(true);
    set_var_b_hide_numpad(true);

    if(!bSaveOnUnload ) {
      bSaveOnUnload = true;
    }
  }
}

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

bool requestAndValidateConfiguration(int16_t id, const String &description, unsigned long startTime, unsigned long globalTimeout) {
    // Anfrage senden
    String response;
    if(DEBUG) Serial.println("Sende Anfrage für: " + description);
    bool ack = uartCom.sendData('R', id, "", true, 3); // Anfrage senden (Read)
    if(!ack){
      uartCom.reset();
      delay(100);
      ack = uartCom.sendData('R', id, "", true, 3);
      if(!ack) return false;
    }

    // Warte auf die Antwort
    while (millis() - startTime < globalTimeout) {
        lv_task_handler();
        ui_tick();      // Display aktualisieren
        uartCom.tick(); // Eingehende Nachrichten prüfen

        if (uartCom.getRequestResponse(id, response)) {
            if (!response.isEmpty()) {
                if(DEBUG) Serial.println(description + " empfangen: " + response);

                switch(id) {
                  case COM_ID_TYPE:
                    // TOP
                    break;
                  case COM_ID_AKKU_MIN:
                    config.akkuMinV = response.toInt();
                    break;
                  case COM_ID_AKKU_CALIB:
                    // TODO
                    break;
                  case COM_ID_BEEP:
                    config.beep = response.toInt();
                    break;
                  case COM_ID_PWR_MANUEL:
                    config.pumpPwrManu = response.toInt();
                    break;
                  case COM_ID_FLOW_TICKS:
                    config.flowTicks = response.toInt();
                    break;
                  case COM_ID_PRESSURE_AVG:
                    config.pressureAvg = response.toInt();
                    break;
                  case COM_ID_SYSTEM_OFF:
                    config.sysPowerOffTime = response.toInt();
                    break;
                  default:
                    // return false;
                    break;

                }

                return true; // Erfolg
            }
        }

        // delay(50); // Kurze Pause, um die Verarbeitung zu entlasten
    }

    if(DEBUG) Serial.println("Fehler: Keine Antwort für " + description + " erhalten.");
    return false; // Fehler nach Timeout
}

// JSON laden
bool loadConfig() {
    // Öffne die Datei im LittleFS-Dateisystem
    File file = LittleFS.open(configFilePath, "r");
    if (!file) {
        if(DEBUG) Serial.println("Konfigurationsdatei konnte nicht geöffnet werden!");
        return false;
    }

    // JSON-Parsing
    StaticJsonDocument<512> doc;  // Größe je nach JSON-Daten anpassen
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        if(DEBUG) Serial.println("JSON-Parsing fehlgeschlagen!");
        file.close();
        return false;
    }

    // Daten ins Config-Struct laden
    config = {
      .owner = doc["owner"].as<String>(),
      .lastModel = doc["lastModel"],
      .beep = doc["beep"],
      .flowTicks = doc["flowTicks"],
      .pressureAvg = doc["pressureAvg"],
      .akkuMinV = doc["akkuMinV"],
      .akkuFactor = doc["akkuFactor"],
      .sysPowerOffTime = doc["sysPowerOffTime"],
      .pumpPwrManu = doc["pumpPwrManu"],
      .pumpPwrCalib = doc["pumpPwrCalib"]
    };

    file.close();
    if(DEBUG) Serial.println("Konfiguration erfolgreich geladen!");
    return true;
}

// JSON speichern
bool saveConfig() {
    // Erstelle oder überschreibe die Datei
    File file = LittleFS.open(configFilePath, "w");
    if (!file) {
        if(DEBUG) Serial.println("Konfigurationsdatei konnte nicht erstellt werden!");
        return false;
    }

    // JSON-Generierung
    StaticJsonDocument<512> doc;  // Größe je nach JSON-Daten anpassen
    doc["owner"] = config.owner;
    doc["lastModel"] = config.lastModel;
    doc["beep"] = config.beep;
    doc["flowTicks"] = config.flowTicks;
    doc["pressureAvg"] = config.pressureAvg;
    doc["akkuMinV"] = config.akkuMinV;
    doc["akkuFactor"] = config.akkuFactor;
    doc["sysPowerOffTime"] = config.sysPowerOffTime;
    doc["pumpPwrManu"] = config.pumpPwrManu;
    doc["pumpPwrCalib"] = config.pumpPwrCalib;

    // Schreibe JSON in die Datei
    if (serializeJson(doc, file) == 0) {
        if(DEBUG) Serial.println("JSON-Serialisierung fehlgeschlagen!");
        file.close();
        return false;
    }

    file.close();
    if(DEBUG) Serial.println("Konfiguration erfolgreich gespeichert!");
    return true;
}

/**
 * Modell (Button) in Auflistung einfügen
 */
lv_obj_t* addModelButton2container(String name, int id) {
  // Modelle einfügen
  name.trim();
  lv_obj_t *parent_obj = objects.cont_models;
  {
    lv_obj_t *obj = lv_btn_create(parent_obj);
    lv_obj_set_pos(obj, -194, 156);
    lv_obj_set_size(obj, 378, 109);
    lv_obj_add_event_cb(obj, action_go2model, LV_EVENT_CLICKED, (void *)id);
    lv_obj_add_event_cb(obj, action_go2settings_models, LV_EVENT_LONG_PRESSED, (void *)id);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(106), LV_SIZE_CONTENT);
            lv_label_set_text(obj, name.c_str());
            add_style_label_model_style(obj);
        }
    }
    return obj;
  }
}


/**
 * Modell Neu "+" (Button) in Auflistung einfügen
 */
lv_obj_t* addModelPlusButton2container(int id) {
  // Modell Neu Button einfügen
  lv_obj_t *parent_obj = objects.cont_models;
  {
      lv_obj_t *obj = lv_btn_create(parent_obj);
      lv_obj_set_pos(obj, 390, 234);
      lv_obj_set_size(obj, 378, 109);
      lv_obj_add_event_cb(obj, action_go2new_model, LV_EVENT_LONG_PRESSED, (void *)id);
      lv_obj_set_user_data(obj, (void*)id);
      lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
      {
          lv_obj_t *parent_obj = obj;
          {
              lv_obj_t *obj = lv_label_create(parent_obj);
              objects.lbl_model_plus = obj;
              lv_obj_set_pos(obj, 0, 1);
              lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
              lv_label_set_text(obj, "+");
              add_style_label_model_style(obj);
              lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
              lv_obj_set_style_text_color(obj, lv_color_hex(0xff5b5b5b), LV_PART_MAIN | LV_STATE_DEFAULT);
          }
      }
      return obj;
  }
}

// Modell laden
bool loadModel(int id){
  String filename;

  if(id < 0) {
    filename = "/initialModel.json";
  } else {
    filename = "/models/" + String(id) + ".json";
  }

  bool loadOk = model.loadFromLittleFS(filename.c_str());
  
  if(loadOk) {
    // Zugriff auf die Parameter
    set_var_s_modelname(model.getModelName().c_str());
    set_var_s_tank_type(getTankTypeDescription(model.getTankType()));
    set_var_i_tank_type_model(model.getTankType());

    // ?? menge
    //lv_scale_set_range(objects.scale_ml, 0, doc["menge"]);
    //set_var_i_fuel_ml(doc["menge"]);

    set_var_s_pump_pwr(int2char(model.getPumpPwr(), LBL_POSTFIX_PROZENT));
    set_var_i_pump_pwr(model.getPumpPwr());
    set_var_s_pump_pwr_model(int2char(model.getPumpPwr()));

    set_var_s_pressure_drop_hose_break(int2char(model.getPressureDropHoseBreak()));
    set_var_s_fueling_last(int2char(model.getFuelingLast()));
    set_var_s_fueling_count(int2char(model.getFuelingCount()));
    set_var_s_fueling_total(String(model.getFuelingTotal() / 1000.0).c_str());
    set_var_s_max_refuel_time(int2char(model.getMaxRefuelTime()));
    set_var_s_max_defuel_time(int2char(model.getMaxDefuelTime()));
    set_var_s_back_fuel_time(int2char(model.getBackFuelTime()));
    set_var_s_air_removal_time(int2char(model.getAirRemovalTime()));
    set_var_s_pump_stop_empty_delay(int2char(model.getPumpStopEmptyDelay()));
    set_var_s_messurement_delay(int2char(model.getMessurementDelay()));
    set_var_s_max_refuel_ml(int2char(model.getMaxRefuelMl()));
    set_var_s_max_defuel_ml(int2char(model.getMaxDefuelMl()));
    set_var_s_max_pressure(int2char(model.getMaxPressure()));
    set_var_s_pump_stop_pressure_diff(int2char(model.getPumpStopPressureDiff()));
    set_var_s_pump_stop_pressure_empty(int2char(model.getPumpStopPressureEmpty()));
    set_var_s_hopper_pressure(int2char(model.getHopperPressure()));
    set_var_s_pump_stop_hopper_pressure_diff(int2char(model.getPumpStopHopperPressureDiff()));

    /*
    modelParams.modelName;
    modelParams.tankType;
    modelParams.menge;
    modelParams.pumpPwr;
    modelParams.pressureDropHoseBreak;
    modelParams.fuelingLast;
    modelParams.fuelingCount;
    modelParams.fuelingTotal;
    modelParams.maxRefuelTime;
    modelParams.maxDefuelTime;
    modelParams.backFuelTime;
    modelParams.airRemovalTime;
    modelParams.pumpStopEmptyDelay;
    modelParams.messurementDelay;
    modelParams.maxRefuelMl;
    modelParams.maxDefuelMl;
    modelParams.maxPressure;
    modelParams.pumpStopPressureDiff;
    modelParams.pumpStopPressureEmpty;
    modelParams.hopperPressure;
    modelParams.pumpStopHopperPressureDiff;
    */

  }

  return loadOk;
}

// Modellparameter aus JSON-Datei laden
bool loadModelParameters(const String &filename, StaticJsonDocument<1024> &doc) {
    File file = LittleFS.open(filename, "r");
    if (!file) {
        if(DEBUG) Serial.println("Fehler: Datei konnte nicht geöffnet werden: " + filename);
        return false;
    }

    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        if(DEBUG) Serial.println("Fehler beim Parsen der JSON-Datei: " + filename);
        return false;
    }

    if(DEBUG) Serial.println("Modellparameter erfolgreich geladen aus: " + filename);
    return true;
}

bool saveModel(const String &filename) {

  // Zugriff auf die Parameter
  model.setModelName(                   get_var_s_modelname());
  model.setTankType(                    (TankTypeEnum )get_var_i_tank_type_model());

  if(model.getTankType() == TANK_BEUTEL) {

  }
  // ?? menge
  //lv_scale_set_range(objects.scale_ml, 0, doc["menge"]);
  //set_var_i_fuel_ml(doc["menge"]);

  model.setPumpPwr(                     getValue(get_var_s_pump_pwr_model(), 60, 25, 100));
  model.setPressureDropHoseBreak(       getValue(get_var_s_pressure_drop_hose_break(), 0, 0, 255));
  model.setFuelingLast(                 atoi(get_var_s_fueling_last()));
  model.setFuelingCount(                atoi(get_var_s_fueling_count()));
  model.setFuelingTotal(                String(get_var_s_fueling_total()).toFloat() * 1000);
  model.setMaxRefuelTime(               atoi(get_var_s_max_refuel_time()));
  model.setMaxDefuelTime(               atoi(get_var_s_max_defuel_time()));
  model.setBackFuelTime(                atoi(get_var_s_back_fuel_time()));
  model.setAirRemovalTime(              atoi(get_var_s_air_removal_time()));
  model.setPumpStopEmptyDelay(          atoi(get_var_s_pump_stop_empty_delay()));
  model.setMessurementDelay(            atoi(get_var_s_messurement_delay()));
  model.setMaxRefuelMl(                 atoi(get_var_s_max_refuel_ml()));
  model.setMaxDefuelMl(                 atoi(get_var_s_max_defuel_ml()));
  model.setMaxPressure(                 atoi(get_var_s_max_pressure()));
  model.setPumpStopPressureDiff(        atoi(get_var_s_pump_stop_pressure_diff()));
  model.setPumpStopPressureEmpty(       atoi(get_var_s_pump_stop_pressure_empty()));
  model.setHopperPressure(              atoi(get_var_s_hopper_pressure()));
  model.setPumpStopHopperPressureDiff(  atoi(get_var_s_pump_stop_hopper_pressure_diff()));

  // Sicherstellen dass bei Beuteltank die Abschaltzeit LEER immer 0 ist
  if(model.getTankType() == TANK_BEUTEL)
    model.setPumpStopEmptyDelay(0);

  return model.saveToLittleFS(filename.c_str());
}

void deleteModel(lv_obj_t *obj) {

  int id = (int)lv_obj_get_user_data(obj);
  String filename = "/models/" + String(id) + ".json";

  if(DEBUG) Serial.println("remove:     " + filename);
  LittleFS.remove(filename);


  if(obj != objModelPlus) {
    lv_obj_del(obj);
    obj = NULL;
    if(DEBUG) Serial.println("Menü OBJ delete: erledigt");
  } else {
    if(DEBUG) Serial.println("Menü OBJ delete: wird nicht durchgeführt da '+' Button");
  }

}


void btnModelSaveClick() {
  // Modell erstellen/updaten
  int id = (int)lv_obj_get_user_data(objLoadedModel);
  String filename = getModelFilename(id);
  // Modell bereits vorhanden
  bool isNewModel = !LittleFS.exists(filename);
  // Erstelle oder überschreibe die Datei
  bool bNoError = saveModel(filename);
  if(bNoError)
    if(DEBUG) Serial.println("Modell gespeichert: " + filename);
  else
    if(DEBUG) Serial.println("FEHLER nicht gespeichert: " + filename);
  // TODO: Fehler anzeigen

  // Auflistung aktualisieren
  if(isNewModel && bNoError) {
    // neues Modell erstellen und id hochzählen
    objLoadedModel = addModelButton2container(get_var_s_modelname(), id);
    // PLUS Button ans Ende Verschieben und nächste neie id setzen
    lv_obj_set_user_data(objModelPlus, (void*)++id);
    lv_obj_move_foreground(objModelPlus);  // Am Ende platzieren
  } else {
    lv_label_set_text(lv_obj_get_child(objLoadedModel, 0), model.getModelName().c_str());
  }
}

// Modell per UART senden
bool sendModelDataToController() {
    // String json = model.toJSON();
    // return uartCom.sendData('W', COM_ID_MODEL_JSON, json, true);

  bool bOk = true;

  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_TANKTYPE, String(model.getTankType()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_PUMP_PWR, String(model.getPumpPwr()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_PRESSURE_DROP_HOSE_BREAK, String(model.getPressureDropHoseBreak()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_MAX_REFUEL_TIME, String(model.getMaxRefuelTime()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_MAX_DEFUEL_TIME, String(model.getMaxDefuelTime()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_BACK_FUEL_TIME, String(model.getBackFuelTime()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_AIR_REMOVAL_TIME, String(model.getAirRemovalTime()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_PUMP_STOP_EMPTY_DELAY, String(model.getPumpStopEmptyDelay()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_MESSUREMENT_DELAY, String(model.getMessurementDelay()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_MAX_REFUEL_ML, String(model.getMaxRefuelMl()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_MAX_DEFUEL_ML, String(model.getMaxDefuelMl()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_MAX_PRESSURE, String(model.getMaxPressure()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_PUMP_STOP_PRESSURE_DIFF, String(model.getPumpStopPressureDiff()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_PUMP_STOP_PRESSURE_EMPTY, String(model.getPumpStopPressureEmpty()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_HOPPER_PRESSURE, String(model.getHopperPressure()), true, 5);
  if(bOk)
    bOk = uartCom.sendData('W', COM_ID_PUMP_STOP_HOPPER_PRESSURE_DIFF, String(model.getPumpStopHopperPressureDiff()), true, 5);

  return bOk;
}

void viewModelParameters(TankTypeEnum tankType) {

  // Alle einblenden
  set_var_b_hide_mod_menge(false);
  set_var_b_hide_mod_pump_pwr(false);
  set_var_b_hide_mod_drop_hose_break(false);

  set_var_b_hide_mod_max_refuel_time(false);
  set_var_b_hide_mod_max_defuel_time(false);
  set_var_b_hide_mod_back_fuel_time(false);
  set_var_b_hide_mod_air_removal_time(false);
  set_var_b_hide_mod_pump_stop_empty_delay(false);
  set_var_b_hide_mod_messurement_delay(false);

  set_var_b_hide_mod_max_refuel_ml(false);
  set_var_b_hide_mod_max_defuel_ml(false);
  set_var_b_hide_mod_max_pressure(false);
  set_var_b_hide_mod_pump_stop_pressure_diff(false);
  set_var_b_hide_mod_pump_stop_pressure_empty(false);
  set_var_b_hide_mod_hopper_pressure(false);
  set_var_b_hide_mod_pump_stop_hopper_pressure_diff(false);

  set_var_b_hide_mod_fueling_last(false);
  set_var_b_hide_mod_fueling_count(false);
  set_var_b_hide_mod_fueling_total(false);
  // Beutel Informationen ausblenden
  set_var_b_hide_mod_bag_info(true);
      
  // Entsprechend des Tanktyps wieder ausblenden
  switch (tankType) {
    case TANK_BEUTEL:
      set_var_b_hide_mod_menge(true);
      set_var_b_hide_mod_drop_hose_break(true);

      set_var_b_hide_mod_pump_stop_empty_delay(true);

      set_var_b_hide_mod_pump_stop_pressure_diff(true);
      set_var_b_hide_mod_hopper_pressure(true);
      set_var_b_hide_mod_pump_stop_hopper_pressure_diff(true);

      set_var_b_hide_mod_fueling_last(true);
      set_var_b_hide_mod_fueling_count(true);
      set_var_b_hide_mod_fueling_total(true);
      // Beutel Informationen einblenden
      set_var_b_hide_mod_bag_info(false);
      break;

    case TANK_1TANK:
      set_var_b_hide_mod_menge(true);
      set_var_b_hide_mod_drop_hose_break(true);

      set_var_b_hide_mod_air_removal_time(true);

      set_var_b_hide_mod_hopper_pressure(true);
      set_var_b_hide_mod_pump_stop_hopper_pressure_diff(true);

      set_var_b_hide_mod_fueling_last(true);
      set_var_b_hide_mod_fueling_count(true);
      set_var_b_hide_mod_fueling_total(true);
      break;

    case TANK_2TANK:
      set_var_b_hide_mod_air_removal_time(true);

      set_var_b_hide_mod_fueling_last(true);
      set_var_b_hide_mod_fueling_count(true);
      set_var_b_hide_mod_fueling_total(true);
      break;
  }

}