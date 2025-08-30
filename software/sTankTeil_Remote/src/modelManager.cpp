#include "modelManager.h"
#include "helper.h"
#include "ui/vars.h"
#include "main.h"
#include "uartCommunication.h"
#include "modelParameters.h"
#include "configManager.h"
#include <esp_task_wdt.h>
#include <LittleFS.h>
#include "ui/actions.h"
#include "ui/screens.h" // Füge dieses Include hinzu, damit 'objects' bekannt ist
#include "ui/styles.h" // Add this include so the linker finds add_style_label_model_style

// Externe globale Variablen/Objekte aus main.cpp
extern lv_obj_t *objModelPlus;
extern void add_style_label_model_style(lv_obj_t *obj);
extern void action_go2model(lv_event_t *e);
extern void action_go2settings_models(lv_event_t *e);
extern void action_go2new_model(lv_event_t *e);

/** Modell laden */
bool loadModel(int id, ModelParameters &model) {
    String filename;
    if(id < 0) {
        filename = "/initialModel.json";
        if(DEBUG) Serial.println("Loading initial model from: " + filename);
    } else {
        filename = "/models/" + String(id) + ".json";
        if(DEBUG) Serial.println("Loading model from: " + filename);
    }
    if(!LittleFS.exists(filename)) {
        if(DEBUG) Serial.println("Error: Model file doesn't exist: " + filename);
        return false;
    }
    bool loadOk = model.loadFromLittleFS(filename.c_str());
    if(DEBUG && loadOk) Serial.println("Model loaded successfully: " + model.getModelName());

        // Zugriff auf die Parameter
    set_var_s_modelname(model.getModelName().c_str());
    set_var_s_tank_type(getTankTypeDescription(model.getTankType()));
    set_var_i_tank_type_model(model.getTankType());

    set_var_s_pump_pwr(int2char(model.getPumpPwr(), LBL_POSTFIX_PROZENT));
    set_var_i_pump_pwr(model.getPumpPwr());
    set_var_s_pump_pwr_model(int2char(model.getPumpPwr()));

    set_var_s_pressure_drop_hose_break(int2char(model.getPressureDropHoseBreak()));
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
    
    return loadOk;
}

/** Modell speichern */
bool saveModel(int id, ModelParameters &model) {
    String filename = "/models/" + String(id) + ".json";
    return model.saveToLittleFS(filename.c_str());
}

/** Modell löschen */
void deleteModel(int id) {
    String filename = "/models/" + String(id) + ".json";
    if(DEBUG) Serial.println("Removing file: " + filename);
    if (LittleFS.exists(filename)) {
        bool removed = LittleFS.remove(filename);
        if(DEBUG) {
            if (removed) Serial.println("File successfully removed: " + filename);
            else Serial.println("Error: Failed to remove file: " + filename);
        }
    } else {
        if(DEBUG) Serial.println("Warning: File does not exist: " + filename);
    }
}

/** Modelle aus Storage laden und Buttons erzeugen */
void loadModelsFromStorage() {
    // Make sure models directory exists
    if (!LittleFS.exists("/models")) {
        if(DEBUG) Serial.println("Creating models directory...");
        if (!LittleFS.mkdir("/models")) {
            if(DEBUG) Serial.println("Failed to create models directory!");
        }
    }

    File root = LittleFS.open("/models/");
    if (!root) {
        if(DEBUG) Serial.println("Failed to open models directory");
        return;
    }

    if (!root.isDirectory()) {
        if(DEBUG) Serial.println("Error: /models/ is not a directory");
        root.close();
        return;
    }

    int modelCount = 0;
    File file = root.openNextFile();
    int maxId = 1;
    int curId;

    if(DEBUG) Serial.println("Starting to scan models directory...");

    while (file) {
        String fileName = file.name();
        if(DEBUG) Serial.println("Found file: " + fileName);

        // Skip files that don't have numeric names or hidden files
        if (!isDigit(fileName[0])) {
            if(DEBUG) Serial.println("Skipping non-numeric filename: " + fileName);
            file.close();
            file = root.openNextFile();
            continue;
        }

        curId = fileName.toInt();
        if(curId > maxId) maxId = curId;

        size_t fileSize = file.size();
        if(DEBUG) Serial.println("File size: " + String(fileSize) + " bytes");

        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, file);

        if (!error) {
            String modelName = doc["modelName"].as<String>();
            if(DEBUG) Serial.println("Model ID: " + String(curId) + ", Name: " + modelName);

            addModelButton2container(modelName, curId);
            modelCount++;
        } else {
            if(DEBUG) Serial.println("Error parsing model file: " + fileName + " - " + String(error.c_str()));
        }

        file.close();
        file = root.openNextFile();

        // Periodically update display and reset watchdog
        if (file && (modelCount % 3 == 0)) {
            lv_timer_handler();
            esp_task_wdt_reset();
        }
    }

    if(DEBUG) Serial.println("Loaded " + String(modelCount) + " models, max ID: " + String(maxId));
    objModelPlus = addModelPlusButton2container(++maxId);
    root.close();
}

/** Modell-Button erzeugen */
lv_obj_t* addModelButton2container(String name, int id) {
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

/** Modell-Plus-Button erzeugen */
lv_obj_t* addModelPlusButton2container(int id) {
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

/** Modellparameter laden */
bool loadModelParameters(const String &filename, StaticJsonDocument<1024> &doc) {
    File file = LittleFS.open(filename, "r");
    if (!file) {
        Serial.println("Fehler: Datei konnte nicht geöffnet werden: " + filename);
        return false;
    }

    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Serial.println("Fehler beim Parsen der JSON-Datei: " + filename);
        return false;
    }

    Serial.println("Modellparameter erfolgreich geladen aus: " + filename);
    return true;
}

/** Modell speichern Button Click */
void btnModelSaveClick(lv_obj_t *objLoadedModel, lv_obj_t *objModelPlus, ModelParameters &model) {
    int id = (int)lv_obj_get_user_data(objLoadedModel);
    String filename = getModelFilename(id);
    bool isNewModel = !LittleFS.exists(filename);

    // Parameter ins ModelParameters-Objekt schreiben
    model.setModelName(get_var_s_modelname());
    model.setTankType((TankTypeEnum)get_var_i_tank_type_model());
    model.setPumpPwr(getValue(get_var_s_pump_pwr_model(), 60, 25, 100));
    model.setPressureDropHoseBreak(getValue(get_var_s_pressure_drop_hose_break(), 0, 0, 255));
    model.setMaxRefuelTime(atoi(get_var_s_max_refuel_time()));
    model.setMaxDefuelTime(atoi(get_var_s_max_defuel_time()));
    model.setBackFuelTime(atoi(get_var_s_back_fuel_time()));
    model.setAirRemovalTime(atoi(get_var_s_air_removal_time()));
    model.setPumpStopEmptyDelay(atoi(get_var_s_pump_stop_empty_delay()));
    model.setMessurementDelay(atoi(get_var_s_messurement_delay()));
    model.setMaxRefuelMl(atoi(get_var_s_max_refuel_ml()));
    model.setMaxDefuelMl(atoi(get_var_s_max_defuel_ml()));
    model.setMaxPressure(atoi(get_var_s_max_pressure()));
    model.setPumpStopPressureDiff(atoi(get_var_s_pump_stop_pressure_diff()));
    model.setPumpStopPressureEmpty(atoi(get_var_s_pump_stop_pressure_empty()));
    model.setHopperPressure(atoi(get_var_s_hopper_pressure()));
    model.setPumpStopHopperPressureDiff(atoi(get_var_s_pump_stop_hopper_pressure_diff()));

    // Sicherstellen dass bei Beuteltank die Abschaltzeit LEER immer 0 ist
    if(model.getTankType() == TANK_BEUTEL)
        model.setPumpStopEmptyDelay(0);

    // NEU: Modell speichern über modelManager
    bool bNoError = saveModel(id, model);

    if(bNoError)
        if(DEBUG) Serial.println("Modell gespeichert: " + filename);
    else
        if(DEBUG) Serial.println("FEHLER nicht gespeichert: " + filename);

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

/** Modell Parameter an Controller senden */
bool sendModelDataToController(const ModelParameters &model) {
    extern UartCommunication uartCom;
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

/** Modell Parameter entsprechend des Tanktyps ein-/ausblenden */
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

            // Beutel Informationen einblenden
            set_var_b_hide_mod_bag_info(false);
            break;

        case TANK_1TANK:
            set_var_b_hide_mod_menge(true);
            set_var_b_hide_mod_drop_hose_break(true);

            set_var_b_hide_mod_air_removal_time(true);

            set_var_b_hide_mod_hopper_pressure(true);
            set_var_b_hide_mod_pump_stop_hopper_pressure_diff(true);
            break;

        case TANK_2TANK:
            set_var_b_hide_mod_air_removal_time(true);
            break;
    }
}
