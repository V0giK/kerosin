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

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>  // Für den Dateizugriff
#include "main.h"
// #include <lvgl.h>
// #include "lovyanGfxSetup.h"
#include "ui/ui.h"
#include "ui/vars.h"
#include "ui/styles.h"
#include "ui/actions.h"
#include "helper.h"
#include "uartCommunication.h"
#include "modelParameters.h"
#include "snakeGame.h"
#include "aeroBlocks.h"  // Include the AeroBlocks header
#include <esp_task_wdt.h>
#include "configManager.h" // Include the config manager
#include "modelManager.h" // Include the model manager
#include "controllerUtils.h" // Include the controller utility functions
#include "uiHandlers.h" // <--- Neues Include für die ausgelagerte Funktion
#include "displayConfig.h" // LVGL Display-Konfiguration
#include "pumpControl.h" // Pumpensteuerung
#include "inputHandlers.h" // Neue Includes für die Input Handler
#include <WiFi.h>
#include <esp_bt.h>

// Konstanten für optimale Timing-Intervalle
const uint32_t UART_CHECK_INTERVAL = 10;     // 100Hz für UART
const uint32_t UI_UPDATE_INTERVAL = 20;      // 50Hz für Display
const uint32_t STATUS_CHECK_INTERVAL = 1000; // 1Hz für System Status
const uint32_t WDT_TIMEOUT = 5;              // 5s Watchdog

// Timing Variablen
static uint32_t lastUartCheck = 0;
static uint32_t lastUiUpdate = 0;
static uint32_t lastStatusCheck = 0;

// Debug-Ausgabe steuern
const bool DEBUG = false;
//LV_LOG_USER("dfg");

// Setup constants
const int UI_UPDATE_ITERATIONS = 5;
const int MODEL_JSON_DOC_SIZE = 1024;  // Increased from 512 to 1024 bytes to handle larger model files
const int SETUP_DISPLAY_DELAY = 100;  // ms

// Display
LGFX tft;

extern lv_event_t g_keyboardShowE;
extern lv_event_t g_numpadShowE;
extern lv_event_t g_go2settingsModelsE;
extern lv_event_t g_resetDataFieldsE;
extern lv_event_t g_go2modelE;
extern lv_event_t g_go2calibrateModelE;
extern lv_event_t g_eventButtonClickE;
extern lv_event_t g_changePumpPwrE;
extern lv_event_t g_go2newModelE;


// Define the global variables that replace the bit fields for the undefined references
// Note: Some flags are now in the UIFlags union - separate bools removed for consistency

volatile UIFlags g_uiflags = {0};  // Init all to 0

lv_obj_t *objModelPlus = NULL;
lv_obj_t *objLoadedModel = NULL;

bool bSaveOnUnload = true; // Werte beim verlassen der Page speichern

// Pfad zur JSON-Konfigurationsdatei
const char *configFilePath = "/config.json";

// Globale Konfigurationsdaten
Config config;

// Modell Daten
ModelParameters model;

// UART-Kommunikation
UartCommunication uartCom(17, 18, DEBUG, 2); // RX = GPIO19, TX = GPIO20

// Snake
SnakeGame snakeGame;
// AeroBlocks
AeroBlocks aeroBlocks;

// Add missing error state flag
bool inControllerErrorState = false;

// Backup mode flags
volatile bool inBackupMode = false;
volatile bool otaCompletedFlag = false;

volatile bool g_modelLoadPending = false;
int g_modelLoadId = 0;



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

void setup() {
    // Disable WiFi and Bluetooth to ensure they are not active
    WiFi.mode(WIFI_OFF);
    WiFi.disconnect(true);
    btStop();

    // Initialize serial communication first
    Serial.begin(115200);
    uartCom.begin(9600); // 19200

    if(DEBUG) Serial.println("Starting initialization...");

    // Display and touch setup
    lvglSetup();
    ui_init();

    // Initial values
    setInitialValues();
    
    // Initialize filesystem
    if (!initializeFileSystem()) {
        showErrorAndSleep("File system error");
        return;
    }
    
    // Update display to show progress
    updateDisplay(UI_UPDATE_ITERATIONS);

    // Load configuration
    loadConfigWithDefaults(config, configFilePath);
    applyConfigToUI(config);
    
    if(DEBUG) printSystemSettings(config);

    // Connect to controller - this can take up to 15 seconds
    bool controllerConnected = searchAndLoadController();
    
    // Initialize watchdog AFTER controller search to prevent timeouts
    esp_task_wdt_init(WDT_TIMEOUT, true);
    esp_task_wdt_add(NULL);
    
    if (!controllerConnected) {
        handleControllerError();
        return;
    }
    
    // Load model data
    if(DEBUG) Serial.println("Loading models...");
    set_var_s_status("Loading models...");
    
    loadModelsFromStorage();
    
    // Display final status of model loading
    if(DEBUG) {
        if(objModelPlus != NULL) {
            Serial.println("Model loader completed successfully");
        } else {
            Serial.println("WARNING: Model loader failed to create the + button");
        }
    }
    
    updateDisplay(10); // More UI updates to ensure models display
    set_var_s_status("");

    // Navigate to appropriate screen
    navigateToInitialScreen();
    
    if(DEBUG) Serial.println("Initialization complete");
}

// Dateisystem initialisieren
bool initializeFileSystem() {
    if (!LittleFS.begin()) {
        if(DEBUG) Serial.println("LittleFS could not be initialized!");
        return false;
    }
    return true;
}

// Controller error handling
void handleControllerError() {
    if(DEBUG) Serial.println("Controller not found or communication error");
    
    // Display error message
    set_var_s_status("Controller not found");
    set_var_b_hide_box_start_error(false);
    
    // Set flag to indicate we're in error state instead of entering infinite loop
    inControllerErrorState = true;
    
    // Update display to ensure the error message is visible
    for(int i = 0; i < 20; i++) {
        lv_timer_handler(); // Use only lv_timer_handler() for LVGL 9.x
        ui_tick();
        delay(10);
    }
}

// Navigate to initial screen based on config
void navigateToInitialScreen() {
    // Go to last model or home screen
    if(config.lastModel > 0) {
        UI_SET_GO2MODEL();
    } else {
        UI_SET_GO2HOME();
    }
}

// Hauptprogramm Loop
void loop() {
    uint32_t currentMillis = millis();
    
    // Reset watchdog first thing to prevent resets
    esp_task_wdt_reset();
    
    // Check if we're in controller error state
    if (inControllerErrorState) {
        // Just keep the UI responsive and reset watchdog
        lv_timer_handler(); // Use only lv_timer_handler() for LVGL 9.x
        ui_tick();
        uartCom.tick();
        delay(50);
        // Backup-Webserver weiter bedienen, falls aktiv
        handleBackupServerLoop();
        return; // Skip the rest of the loop
    }

    // Check if we're in backup mode (OTA optimized)
    if (inBackupMode) {
        // Reset watchdog to prevent timeouts during long OTA transfer
        esp_task_wdt_reset();

        // Minimal UI updates to keep screen responsive
        lv_timer_handler(); // LVGL timer for display updates
        ui_tick();

        // Only run backup server and handle OTA to free maximum memory
        handleBackupServerLoop();

        // Check if OTA completed
        if (otaCompletedFlag) {
            inBackupMode = false;
            otaCompletedFlag = false;

            // Restore original WDT timeout
            esp_task_wdt_init(WDT_TIMEOUT, true); // Back to 5 seconds
            esp_task_wdt_add(NULL);

            ESP.restart();
        }

        vTaskDelay(10); // Small delay to throttle
        return; // Skip all other functionality
    }

    // 1. UART Kommunikation (höchste Priorität - 100Hz)
    if (currentMillis - lastUartCheck >= UART_CHECK_INTERVAL) {
        lastUartCheck = currentMillis;
        uartCom.tick();  // Verarbeite UART-Daten
    }

    // 2. UI Updates (50Hz)
    if (currentMillis - lastUiUpdate >= UI_UPDATE_INTERVAL) {
        lastUiUpdate = currentMillis;
        lv_timer_handler();     // LVGL Timer Handler - use only this one
        ui_tick();

        //updateUIElements();     // Update Display Elements
        handleScreenFlags();
        handlePumpControl();
        handleManualPump();
        handleButtonClick();
        handleSettingsPage();
        handleKeyboard();
        handleNumpad();
    }

    // 3. System Status (1Hz)
    if (currentMillis - lastStatusCheck >= STATUS_CHECK_INTERVAL) {
        lastStatusCheck = currentMillis;
        //checkSystemStatus();    // Prüfe System Parameter
        esp_task_wdt_reset();  // Reset Watchdog
    }

    // Backup-Webserver bedienen, falls aktiv
    handleBackupServerLoop();

    // Yield für andere Tasks
    vTaskDelay(1);
    
}

void deleteModel(lv_obj_t *obj) {
  if (!obj) {
    if(DEBUG) Serial.println("Error: NULL object passed to deleteModel()");
    return;
  }

  int id = (int)lv_obj_get_user_data(obj);
  if(DEBUG) Serial.println("Deleting model with ID: " + String(id));

  // Modell im Dateisystem löschen über modelManager.cpp
  deleteModel(id);

  // Handle the UI element removal
  if(obj != objModelPlus) {
    if(DEBUG) Serial.println("Deleting UI element");
    lv_obj_t* parent = lv_obj_get_parent(obj);
    lv_obj_del(obj);
    obj = NULL;
    
    // Force a redraw of the parent container
    if (parent) {
      lv_obj_invalidate(parent);
    }
    if(DEBUG) Serial.println("Menu OBJ delete: completed");
  } else {
    if(DEBUG) Serial.println("Menu OBJ delete: not performed as it's the '+' button");
  }

  // UI neu laden, um konsistente Model-Liste und Plus-Button zu haben
  loadModelsFromStorage();

  // If this was the last loaded model, reset the config
  if (config.lastModel == id) {
    config.lastModel = 0;
    saveConfig(config, configFilePath);
    if(DEBUG) Serial.println("Reset last loaded model in config");
  }
}

// Ende