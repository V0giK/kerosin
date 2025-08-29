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
#include "modelParameters.h"
#include "ui/vars.h"
#include "ui/screens.h"
#include "ui/actions.h"
#include "snakeGame.h"
#include "configManager.h" // Add this include for Config type
#include <WiFi.h>
#include <WebServer.h>
#include <ElegantOTA.h>
#include <LittleFS.h>
#include <DNSServer.h>
#include <esp_mac.h> // Für MAC-Adresse

WebServer backupServer(80);
DNSServer captiveDns;
const byte DNS_PORT = 53;
bool backupServerRunning = false;

void handleBackupWebServer() {
    if (backupServerRunning) return;

    // Eindeutige SSID generieren (z.B. MAC-Adresse als Suffix)
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    char macStr[13];
    sprintf(macStr, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    String ssid = "KerosinBackup-" + String(macStr);

    // Start WiFi AP mit eindeutiger SSID
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid.c_str(), "12345678");

    // Starte DNS-Server für Captive Portal
    captiveDns.start(DNS_PORT, "*", WiFi.softAPIP());

    // --- WICHTIG: Setze DNS auf die eigene IP für alle Anfragen ---
    // Dies sorgt dafür, dass alle DNS-Anfragen (z.B. zu www.de) auf die ESP32-IP zeigen.
    // captiveDns.start(DNS_PORT, "*", WiFi.softAPIP()); // ist bereits gesetzt

    // --- TFT Hinweis-Screen anzeigen ---
    lv_obj_t* scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x222222), 0);

    lv_obj_t* label1 = lv_label_create(scr);
    lv_label_set_text(label1, "Achtung: WiFi ist jetzt aktiv!");
    lv_obj_set_style_text_color(label1, lv_color_hex(0xFF4444), 0);
    lv_obj_set_style_text_font(label1, &lv_font_montserrat_24, 0);
    lv_obj_align(label1, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_t* label2 = lv_label_create(scr);
    lv_label_set_text_fmt(label2, "SSID: %s", ssid.c_str());
    lv_obj_set_style_text_color(label2, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(label2, &lv_font_montserrat_18, 0);
    lv_obj_align(label2, LV_ALIGN_TOP_MID, 0, 100);

    lv_obj_t* label3 = lv_label_create(scr);
    lv_label_set_text_fmt(label3, "Passwort: %s", "12345678");
    lv_obj_set_style_text_color(label3, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(label3, &lv_font_montserrat_18, 0);
    lv_obj_align(label3, LV_ALIGN_TOP_MID, 0, 140);

    lv_obj_t* label4 = lv_label_create(scr);
    lv_label_set_text_fmt(label4, "IP: %s", WiFi.softAPIP().toString().c_str());
    lv_obj_set_style_text_color(label4, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(label4, &lv_font_montserrat_18, 0);
    lv_obj_align(label4, LV_ALIGN_TOP_MID, 0, 180);

    lv_obj_t* label5 = lv_label_create(scr);
    lv_label_set_text(label5, "WiFi beenden: Tankstation ausschalten!");
    lv_obj_set_style_text_color(label5, lv_color_hex(0xFFDD44), 0);
    lv_obj_set_style_text_font(label5, &lv_font_montserrat_16, 0);
    lv_obj_align(label5, LV_ALIGN_BOTTOM_MID, 0, -40);

    lv_scr_load(scr);

    // Captive Portal: Leite typische URLs auf die Root-Seite um
    // Zusätzliche Weiterleitungen für Android/Samsung
    backupServer.on("/generate_204", HTTP_ANY, []() { backupServer.sendHeader("Location", "/", true); backupServer.send(302, "text/plain", ""); });
    backupServer.on("/hotspot-detect.html", HTTP_ANY, []() { backupServer.sendHeader("Location", "/", true); backupServer.send(302, "text/plain", ""); });
    backupServer.on("/captive-portal", HTTP_ANY, []() { backupServer.sendHeader("Location", "/", true); backupServer.send(302, "text/plain", ""); });
    backupServer.on("/redirect", HTTP_ANY, []() { backupServer.sendHeader("Location", "/", true); backupServer.send(302, "text/plain", ""); });
    backupServer.on("/wpad.dat", HTTP_ANY, []() { backupServer.sendHeader("Location", "/", true); backupServer.send(302, "text/plain", ""); });
    backupServer.on("/favicon.ico", HTTP_ANY, []() { backupServer.send(204, "text/plain", ""); });

    // Zusätzliche Android/Samsung URLs für Captive Portal
    backupServer.on("/ncsi.txt", HTTP_ANY, []() { backupServer.sendHeader("Location", "/", true); backupServer.send(302, "text/plain", ""); });
    backupServer.on("/connectivity-check.html", HTTP_ANY, []() { backupServer.sendHeader("Location", "/", true); backupServer.send(302, "text/plain", ""); });
    backupServer.on("/mobile/status.php", HTTP_ANY, []() { backupServer.sendHeader("Location", "/", true); backupServer.send(302, "text/plain", ""); });
    backupServer.on("/library/test/success.html", HTTP_ANY, []() { backupServer.sendHeader("Location", "/", true); backupServer.send(302, "text/plain", ""); });

    // Fallback: Alle unbekannten URLs auf Root umleiten
    backupServer.onNotFound([]() {
        // Sende eine echte HTML-Seite mit Meta-Refresh für aggressive Weiterleitung
        String html = "<!DOCTYPE html><html><head>";
        html += "<meta http-equiv='refresh' content='0; url=/' />";
        html += "<meta charset='UTF-8'>";
        html += "<title>Kerosin Captive Portal</title>";
        html += "</head><body>";
        html += "<script>window.location.replace('/');</script>";
        html += "<p>Weiterleitung zum Captive Portal...</p>";
        html += "</body></html>";
        backupServer.send(200, "text/html", html);
    });

    // Responsive Root page
    backupServer.on("/", HTTP_GET, []() {
        String html = "<!DOCTYPE html><html><head><title>Kerosin Backup</title>";
        html += "<meta charset='UTF-8'>";
        html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
        html += "<style>";
        html += "body{font-family:sans-serif;background:#222;color:#eee;margin:0;padding:0;}";
        html += ".container{max-width:400px;margin:30px auto;padding:20px;background:#333;border-radius:12px;box-shadow:0 2px 8px #0008;}";
        html += "h2{font-size:2em;text-align:center;margin-bottom:24px;}";
        html += ".section{margin:24px 0;padding:16px 12px;background:#222;border-radius:8px;box-shadow:0 1px 4px #0006;}";
        html += ".section h3{margin-top:0;margin-bottom:12px;font-size:1.3em;color:#8cf;}";
        html += ".section label{display:block;margin-bottom:8px;font-size:1em;}";
        html += "form, .btnlink{display:block;margin:12px 0;text-align:center;}";
        html += "button,input[type=file],select{font-size:1.1em;padding:10px 20px;margin:8px 0;border-radius:8px;border:none;}";
        html += "button{background:#4caf50;color:#fff;box-shadow:0 2px 4px #0004;}";
        html += "button:active{background:#388e3c;}";
        html += "a.btnlink{display:inline-block;background:#2196f3;color:#fff;text-decoration:none;padding:12px 24px;border-radius:8px;box-shadow:0 2px 4px #0004;margin:10px 0;}";
        html += "a.btnlink:active{background:#1565c0;}";
        html += ".footer{text-align:center;font-size:0.9em;color:#aaa;margin-top:24px;}";
        html += "</style></head><body>";
        html += "<div class='container'>";
        html += "<h2>Kerosin Backup/Restore</h2>";

        // Backup section
        html += "<div class='section'>";
        html += "<h3>Backup</h3>";
        html += "<form method='GET' action='/backup'><button>Backup anzeigen</button></form>";
        html += "<small>Hier k&ouml;nnen Sie alle .json Dateien herunterladen.</small>";
        html += "</div>";

        // Restore section
        html += "<div class='section'>";
        html += "<h3>Restore</h3>";
        html += "<form method='POST' action='/restore' enctype='multipart/form-data'>";
        html += "<label for='restore_target'>Restore Ziel:</label>";
        html += "<select name='restore_target' id='restore_target'>";
        html += "<option value='root'>Root-Verzeichnis</option>";
        html += "<option value='models'>/models/ Verzeichnis</option>";
        html += "</select><br>";
        // Setze accept=".json" für die Dateiauswahl
        html += "<input type='file' name='file' multiple accept='.json'><br><button>Restore</button></form>";
        html += "<small>Laden Sie eine oder mehrere .json Dateien hoch und w&auml;hlen Sie das Zielverzeichnis.</small>";
        html += "</div>";

        // OTA section
        html += "<div class='section'>";
        html += "<h3>Firmware Update (OTA)</h3>";
        html += "<a class='btnlink' href='/update'>ElegantOTA &ouml;ffnen</a>";
        html += "<small>Hier k&ouml;nnen Sie die Firmware aktualisieren.</small>";
        html += "</div>";

        html += "<div class='footer'>Projekt <b>kerosin</b> &copy; 2025<br>";
        html += "<a href='https://github.com/V0giK/kerosin' target='_blank'>GitHub: V0giK/kerosin</a></div>";
        html += "</div></body></html>";
        backupServer.send(200, "text/html", html);
    });

    // Backup: list all .json files in / and /models/ and offer as download
    backupServer.on("/backup", HTTP_GET, []() {
        String html = "<!DOCTYPE html><html><head><title>Backup</title>";
        html += "<meta charset='UTF-8'>";
        html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
        html += "<style>";
        html += "body{font-family:sans-serif;background:#222;color:#eee;margin:0;padding:0;}";
        html += ".container{max-width:400px;margin:30px auto;padding:20px;background:#333;border-radius:12px;box-shadow:0 2px 8px #0008;}";
        html += "h2{font-size:2em;text-align:center;margin-bottom:24px;}";
        html += ".filelist{margin:18px 0;}";
        html += ".filelist a{display:block;background:#444;padding:10px 12px;margin:6px 0;border-radius:8px;color:#fff;text-decoration:none;font-size:1.1em;box-shadow:0 1px 2px #0004;}";
        html += ".filelist a:active{background:#2196f3;}";
        html += "a.btnlink,button{display:inline-block;background:#4caf50;color:#fff;text-decoration:none;padding:12px 24px;border-radius:8px;box-shadow:0 2px 4px #0004;margin:10px 0;font-size:1.2em;border:none;}";
        html += "a.btnlink:active,button:active{background:#388e3c;}";
        html += ".footer{text-align:center;font-size:0.9em;color:#aaa;margin-top:24px;}";
        html += "</style></head><body>";
        html += "<div class='container'>";
        html += "<h2>Backup: .json Dateien</h2>";
        html += "<div class='filelist'>";
        // List .json files in root directory
        File rootDir = LittleFS.open("/");
        File file = rootDir.openNextFile();
        while (file) {
            String fname = String(file.name());
            if (fname.endsWith(".json")) {
                html += "<a href='/download?file=" + fname + "' download>ROOT: " + fname + "</a>";
            }
            file.close();
            file = rootDir.openNextFile();
        }
        rootDir.close();
        // List .json files in /models/ directory
        if (LittleFS.exists("/models")) {
            File modelsDir = LittleFS.open("/models");
            File file = modelsDir.openNextFile();
            while (file) {
                String fname = String(file.name());
                // Korrigiere: fname ist z.B. "1.json", aber Download braucht "/models/1.json"
                if (fname.endsWith(".json")) {
                    html += "<a href='/download?file=/models/" + fname + "' download>MODELS: " + fname + "</a>";
                }
                file.close();
                file = modelsDir.openNextFile();
            }
            modelsDir.close();
        }
        html += "</div>";
        html += "<a class='btnlink' href='/'>Zur&uuml;ck</a>";
        html += "<div class='footer'>Projekt <b>kerosin</b> &copy; 2025<br>";
        html += "<a href='https://github.com/V0giK/kerosin' target='_blank'>GitHub: V0giK/kerosin</a></div>";
        html += "</div></body></html>";
        backupServer.send(200, "text/html", html);
    });

    // Download endpoint for individual files
    backupServer.on("/download", HTTP_GET, []() {
        String fname = backupServer.arg("file");
        if (!fname.startsWith("/")) fname = "/" + fname;
        if (!LittleFS.exists(fname)) {
            backupServer.send(404, "text/plain", "File not found: " + fname);
            return;
        }
        File f = LittleFS.open(fname, "r");
        if (!f) {
            backupServer.send(500, "text/plain", "Failed to open file: " + fname);
            return;
        }
        // Setze Content-Disposition für Download mit Originaldateiname
        String originalName = fname.substring(fname.lastIndexOf('/') + 1);
        backupServer.sendHeader("Content-Disposition", "attachment; filename=\"" + originalName + "\"");
        backupServer.streamFile(f, "application/json");
        f.close();
    });

    // Restore: upload .json files to root or /models/ depending on user selection
    backupServer.on("/restore", HTTP_POST, []() {
        String html = "<!DOCTYPE html><html><head><title>Restore</title>";
        html += "<meta charset='UTF-8'>";
        html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
        html += "<style>";
        html += "body{font-family:sans-serif;background:#222;color:#eee;margin:0;padding:0;}";
        html += ".container{max-width:400px;margin:30px auto;padding:20px;background:#333;border-radius:12px;box-shadow:0 2px 8px #0008;}";
        html += "h2{font-size:2em;text-align:center;margin-bottom:24px;}";
        html += ".msg{margin:24px 0;padding:16px 12px;background:#222;border-radius:8px;box-shadow:0 1px 4px #0006;text-align:center;font-size:1.2em;color:#8cf;}";
        html += "a.btnlink{display:inline-block;background:#2196f3;color:#fff;text-decoration:none;padding:12px 24px;border-radius:8px;box-shadow:0 2px 4px #0004;margin:10px 0;}";
        html += "a.btnlink:active{background:#1565c0;}";
        html += ".footer{text-align:center;font-size:0.9em;color:#aaa;margin-top:24px;}";
        html += "</style></head><body>";
        html += "<div class='container'>";
        html += "<h2>Restore</h2>";
        html += "<div class='msg'>Restore erfolgreich abgeschlossen.</div>";
        html += "<a class='btnlink' href='/'>Zur&uuml;ck</a>";
        html += "<div class='footer'>Projekt <b>kerosin</b> &copy; 2025<br>";
        html += "<a href='https://github.com/V0giK/kerosin' target='_blank'>GitHub: V0giK/kerosin</a></div>";
        html += "</div></body></html>";
        backupServer.send(200, "text/html", html);
    }, []() {
        HTTPUpload& upload = backupServer.upload();
        static String restoreTarget = "root";
        // Hole das Restore-Ziel aus dem POST-Body (nur beim ersten Upload-Chunk)
        if (upload.status == UPLOAD_FILE_START) {
            if (backupServer.hasArg("restore_target")) {
                restoreTarget = backupServer.arg("restore_target");
            }
            String path;
            if (restoreTarget == "models") {
                path = "/models/" + upload.filename;
            } else {
                path = "/" + upload.filename;
            }
            // Create parent directory if needed
            int slashIdx = path.lastIndexOf('/');
            if (slashIdx > 0) {
                String dir = path.substring(0, slashIdx);
                if (!LittleFS.exists(dir)) {
                    LittleFS.mkdir(dir);
                }
            }
            File f = LittleFS.open(path, "w");
            f.close();
        } else if (upload.status == UPLOAD_FILE_WRITE) {
            String path;
            if (restoreTarget == "models") {
                path = "/models/" + upload.filename;
            } else {
                path = "/" + upload.filename;
            }
            File f = LittleFS.open(path, "a");
            if (f) f.write(upload.buf, upload.currentSize);
            f.close();
        }
    });

    // ElegantOTA
    ElegantOTA.begin(&backupServer);

    backupServerRunning = true;
    backupServer.begin();
}

void handleBackupServerLoop() {
    if (backupServerRunning) {
        captiveDns.processNextRequest();
        backupServer.handleClient();
    }
}

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
            case BTN_SYSTEM_BACKUP:
                handleBackupWebServer();
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
