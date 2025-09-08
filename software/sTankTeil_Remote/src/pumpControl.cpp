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

#include "pumpControl.h"
#include "main.h"
#include "uartCommunication.h"
#include "ui/vars.h"
#include "ui/screens.h" // already included for SCREEN_ID_PUMP
#include "helper.h"

// Note: UIFlags typedef and g_changePumpPwr, g_go2manuelPump, g_unloadManuelFuel
// are now defined as macros in main.h

// Externe Variablen aus main.cpp
extern UartCommunication uartCom;
extern const bool DEBUG;

// Note: Pump controls now use union bits for consistency

// Add extern declaration for go2screen:
extern void go2screen(ScreensEnum screen);

void handlePumpControl() {
    if (UI_IS_PUMP_IN()) {
        UI_CLEAR_PUMP_IN();

        if(get_var_b_is_pumping()) {
          UI_SET_PUMP_STOP();
        } else {
          set_var_b_is_pumping(true);
          set_var_b_disable_btn_home(true);
          set_var_b_disable_btn_settings(true);
          uartCom.sendData('W', COM_ID_PUMP_PWR, String(get_var_i_pump_pwr()), true);
          uartCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_TANKEN), true);
        }
    }
    if (UI_IS_PUMP_OUT()) {
        UI_CLEAR_PUMP_OUT();

        if(get_var_b_is_pumping()) {
          UI_SET_PUMP_STOP();
        } else {
          set_var_b_is_pumping(true);
          set_var_b_disable_btn_home(true);
          set_var_b_disable_btn_settings(true);
          uartCom.sendData('W', COM_ID_PUMP_PWR, String(get_var_i_pump_pwr()), true);
          uartCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_ENTTANKEN), true);
        }
    }
    if (UI_IS_PUMP_STOP()) {
        UI_CLEAR_PUMP_STOP();
        set_var_b_is_pumping(false);
        // TODO: darauf achten ob bei jeglichem Stop die Buttons wieder aktiviert werden dürfen
        set_var_b_disable_btn_home(false);
        set_var_b_disable_btn_settings(false);
        uartCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP), true);
    }
    if (UI_IS_CHANGE_PUMP_PWR()) {
        UI_CLEAR_CHANGE_PUMP_PWR();
        set_var_s_pump_pwr(int2char(get_var_i_pump_pwr(), LBL_POSTFIX_PROZENT));
        uartCom.sendData('W', COM_ID_PUMP_PWR, String(get_var_i_pump_pwr()), true);
    }
    // Ergänzung: Verbleibende Zeit empfangen und anzeigen
    String uartValue;
    if(uartCom.getRequestResponse(COM_ID_MAX_REFUEL_TIME, uartValue)) {
        if(!uartValue.isEmpty()) {
            set_var_s_rest_tank_time((uartValue + POSTFIX_S).c_str());
        }
    }
    if(uartCom.getRequestResponse(COM_ID_MAX_DEFUEL_TIME, uartValue)) {
        if(!uartValue.isEmpty()) {
            set_var_s_rest_tank_time((uartValue + POSTFIX_S).c_str());
        }
    }
}

void handleManualPump() {
    if (UI_IS_GO2MANUEL_PUMP()) {  // Prüfen, ob das Flag gesetzt ist
        UI_CLEAR_GO2MANUEL_PUMP(); // Flag zurücksetzen

        // Titel für den manuellen Pump-Bildschirm setzen
        set_var_s_screen_titel("Tanken (manuell)");

        // Manuelle Pumpleistung
        set_var_i_pump_pwr(atoi(get_var_s_pump_pwr_manu()));
        set_var_s_pump_pwr(int2char(get_var_i_pump_pwr(), LBL_POSTFIX_PROZENT));

        set_var_s_fuel_ml("0 ml");

        // UI-Elemente anpassen oder anzeigen
        set_var_b_hide_manuel_fuel(false); // Zeige manuelles Tanken-UI an
        uartCom.sendData('W', COM_ID_PUMP_MODE, int2char(MODE_MANUELL), true);
        uartCom.sendData('W', COM_ID_BROADCAST, "1", true);

        // Bildschirm wechseln
        go2screen(SCREEN_ID_PUMP);

        if(DEBUG) Serial.println("Manuelles Pumpen gestartet."); // Debug-Ausgabe
    }

    if (UI_IS_UNLOAD_MANUEL_FUEL()) { // Prüfen, ob das Flag gesetzt ist
        UI_CLEAR_UNLOAD_MANUEL_FUEL(); // Flag zurücksetzen

        // UI-Elemente ausblenden oder anpassen
        set_var_b_hide_cont_model_fuel(true);
        set_var_b_hide_cont_flow_calibrate(true);
        set_var_b_hide_model_fuel(true);
        set_var_b_hide_manuel_fuel(true);

        set_var_b_pump_pwr_disabled(false);

        uartCom.sendData('W', COM_ID_BROADCAST, "0", true);
    }
}