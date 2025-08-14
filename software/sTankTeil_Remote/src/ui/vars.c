#include <string.h>
#include "vars.h"

//extern void printMessage(const char *message);


// System Config
bool b_hide_dialog_save_system;                     // System Speichern Dialog hide/show
char s_owner[100] = { 0 };                          // Eigentümer der Station
bool b_load_last_model;                             // letztes Modell beim Start laden
bool b_signal;                                      // Warnsignale ausgeben (Piezo)
char s_pump_pwr_manu[100] = { 0 };                  // Pumpleistung manuell in %
char s_pump_pwr_calib[100] = { 0 };                 // Pumpleistung Kalibrierbetrieb in %
char s_sys_power_off_time[100] = { 0 };             // Powerofff idle-Time
char s_akku_min_v[100] = { 0 };                     // minimale Akkuspannung
char s_pressure_avg[100] = { 0 };                   // Anzahl Messwerte für Mittelwertbildung
char s_flow_ticks[100] = { 0 };                     // Flowsensor Anzahl Impulse pro Liter

// Allgemeine für Displaysteuerung
char s_screen_titel[100] = { 0 };                   // Seitentitel
char s_akku_volt[100] = { 0 };                      // Akkuspannung
bool b_pump_pwr_disabled;                           // Pumpleistung veränderbar (J/N)
bool b_hide_keyboard;                               // Keyboard hide/show
bool b_hide_numpad;                                 // Numpad hide/show
bool b_disable_btn_settings;                        // Settings Button deaktivieren
bool b_disable_btn_home;                            // Home Button deaktivieren
bool b_hide_box_start_error;                        // Feher Meldung wenn UART beim Start nicht aufgebaut werden kann
bool b_hide_wait;                                   // Warte-Kringel anzeigen

// Pumpensteuerung
bool b_is_pumping;                                  // Ent/Betankung läuft
char s_status[100] = { 0 };                         // Statustext (Bereit, Tanken, Enttanken, Entlüftung, ...)
int32_t i_fuel_ml;                                  // Aktuelle Füllmenge in Milliliter
int32_t i_pump_pwr;                                 // Pumpenleistung in % (Arc einstellen)
char s_pump_pwr[100] = { 0 };                       // Pumpenleistung in % (Anzeige)
bool b_hide_model_fuel;                             // Container Tankung Modell
bool b_hide_manuel_fuel;                            // Contaoner Tankung manuell
bool b_hide_cont_model_fuel;                        // Container Kalibrierung Modell
bool b_hide_cont_flow_calibrate;                    // Container Kalibrierung Druchflusssensor
char i_calib_flow_sensor[100] = { 0 };              // Flowsensor Kalibrierung in Pulse pro Liter
char s_fuel_mbar[100] = { 0 };                      // Tankdruck
char s_fuel_diff_mbar[100] = { 0 };                 // Differenzdruck

// Modell Settings
bool b_hide_dialog_save_model;                      // Modell Speichern Dialog hide/show
char s_modelname[100] = { 0 };                      // Modellname
char s_tank_type[100] = { 0 };                      // Tank type Beutel, 1 Tank, 2 Tanks
int32_t i_tank_type_model;                          // Tanktype
char s_pump_pwr_model[100] = { 0 };                 // Pumpleistung
char s_fueling_total[100] = { 0 };                  // Gesamttankmenege
char s_fueling_count[100] = { 0 };                  // Anzahl Tankungen
char s_fueling_last[100] = { 0 };                   // letzte Tankmenge
char s_max_refuel_time[100] = { 0 };                // Maximale Betankzeit in s
char s_max_defuel_time[100] = { 0 };                // Maximale Enttankzeit in s
char s_back_fuel_time[100] = { 0 };                 // Rücktankzeit in s
char s_air_removal_time[100] = { 0 };               // Luftblasenentfernung in s
char s_pump_stop_empty_delay[100] = { 0 };          // Abschaltzeit nach Leererkennung in s
char s_messurement_delay[100] = { 0 };              // Messverzögerung in s
char s_max_refuel_ml[100] = { 0 };                  // Maximale Betankmenge in ml
char s_max_defuel_ml[100] = { 0 };                  // Maximale Enttankmenge in ml
char s_max_pressure[100] = { 0 };                   // Maximaler Tankdruck in mbar
char s_pump_stop_pressure_diff[100] = { 0 };        // Differenzabschaltdruck in mbar
char s_pump_stop_pressure_empty[100] = { 0 };       // Leerlaufabschaltdruck in mbar
char s_hopper_pressure[100] = { 0 };                // Maximaler Hopperfülldruck in mbar
char s_pump_stop_hopper_pressure_diff[100] = { 0 }; // Differenzabschaltdruck Hopper in mbar
char s_pressure_drop_hose_break[100] = { 0 };       // Druckabfall bei Schlauchbruch in mbar

// Modellparamter ausblenden
bool b_hide_mod_menge;
bool b_hide_mod_pump_pwr;
bool b_hide_mod_drop_hose_break;
bool b_hide_mod_max_refuel_time;
bool b_hide_mod_max_defuel_time;
bool b_hide_mod_back_fuel_time;
bool b_hide_mod_air_removal_time;
bool b_hide_mod_pump_stop_empty_delay;
bool b_hide_mod_messurement_delay;
bool b_hide_mod_max_refuel_ml;
bool b_hide_mod_max_defuel_ml;
bool b_hide_mod_max_pressure;
bool b_hide_mod_pump_stop_pressure_diff;
bool b_hide_mod_pump_stop_pressure_empty;
bool b_hide_mod_hopper_pressure;
bool b_hide_mod_pump_stop_hopper_pressure_diff;
bool b_hide_mod_fueling_last;
bool b_hide_mod_fueling_count;
bool b_hide_mod_fueling_total;
bool b_hide_mod_bag_info;

bool b_hide_pump_save_mod_to_controller;

char s_fuel_ml[100] = { 0 };                        // aktuelle Tankmenge - bei manuell Tanken
char s_cut_off_ml[100] = { 0 };                     // ml bis zum abschalten

/**
 * Funktionen
 */


const char *get_var_s_screen_titel() {
    return s_screen_titel;
}

void set_var_s_screen_titel(const char *value) {
    strncpy(s_screen_titel, value, sizeof(s_screen_titel) / sizeof(char));
    s_screen_titel[sizeof(s_screen_titel) / sizeof(char) - 1] = 0;
}

const char *get_var_s_owner() {
    return s_owner;
}

void set_var_s_owner(const char *value) {
    strncpy(s_owner, value, sizeof(s_owner) / sizeof(char));
    s_owner[sizeof(s_owner) / sizeof(char) - 1] = 0;
}

const char *get_var_s_pressure_avg() {
    return s_pressure_avg;
}

void set_var_s_pressure_avg(const char *value) {
    strncpy(s_pressure_avg, value, sizeof(s_pressure_avg) / sizeof(char));
    s_pressure_avg[sizeof(s_pressure_avg) / sizeof(char) - 1] = 0;
}

const char *get_var_s_akku_min_v() {
    return s_akku_min_v;
}

void set_var_s_akku_min_v(const char *value) {
    strncpy(s_akku_min_v, value, sizeof(s_akku_min_v) / sizeof(char));
    s_akku_min_v[sizeof(s_akku_min_v) / sizeof(char) - 1] = 0;
}

const char *get_var_s_flow_ticks() {
    return s_flow_ticks;
}

void set_var_s_flow_ticks(const char *value) {
    strncpy(s_flow_ticks, value, sizeof(s_flow_ticks) / sizeof(char));
    s_flow_ticks[sizeof(s_flow_ticks) / sizeof(char) - 1] = 0;
}


bool get_var_b_load_last_model() {
    return b_load_last_model;
}

void set_var_b_load_last_model(bool value) {
    b_load_last_model = value;
}

bool get_var_b_signal() {
    return b_signal;
}

void set_var_b_signal(bool value) {
    b_signal = value;
}

bool get_var_b_hide_keyboard() {
    return b_hide_keyboard;
}

void set_var_b_hide_keyboard(bool value) {
    b_hide_keyboard = value;
}

bool get_var_b_hide_numpad() {
    return b_hide_numpad;
}

void set_var_b_hide_numpad(bool value) {
    b_hide_numpad = value;
}

int32_t get_var_i_pump_pwr() {
    return i_pump_pwr;
}

void set_var_i_pump_pwr(int32_t value) {
    i_pump_pwr = value;
}

bool get_var_b_hide_cont_flow_calibrate() {
    return b_hide_cont_flow_calibrate;
}

void set_var_b_hide_cont_flow_calibrate(bool value) {
    b_hide_cont_flow_calibrate = value;
}

bool get_var_b_hide_cont_model_fuel() {
    return b_hide_cont_model_fuel;
}

void set_var_b_hide_cont_model_fuel(bool value) {
    b_hide_cont_model_fuel = value;
}

const char *get_var_i_calib_flow_sensor() {
    return i_calib_flow_sensor;
}

void set_var_i_calib_flow_sensor(const char *value) {
    strncpy(i_calib_flow_sensor, value, sizeof(i_calib_flow_sensor) / sizeof(char));
    i_calib_flow_sensor[sizeof(i_calib_flow_sensor) / sizeof(char) - 1] = 0;
}

const char *get_var_s_pump_pwr() {
    return s_pump_pwr;
}

void set_var_s_pump_pwr(const char *value) {
    strncpy(s_pump_pwr, value, sizeof(s_pump_pwr) / sizeof(char));
    s_pump_pwr[sizeof(s_pump_pwr) / sizeof(char) - 1] = 0;
}

bool get_var_b_is_pumping() {
    return b_is_pumping;
}

void set_var_b_is_pumping(bool value) {
    b_is_pumping = value;
}

bool get_var_b_hide_model_fuel() {
    return b_hide_model_fuel;
}

void set_var_b_hide_model_fuel(bool value) {
    b_hide_model_fuel = value;
}

bool get_var_b_hide_manuel_fuel() {
    return b_hide_manuel_fuel;
}

void set_var_b_hide_manuel_fuel(bool value) {
    b_hide_manuel_fuel = value;
}

const char *get_var_s_status() {
    return s_status;
}

void set_var_s_status(const char *value) {
    strncpy(s_status, value, sizeof(s_status) / sizeof(char));
    s_status[sizeof(s_status) / sizeof(char) - 1] = 0;
}

int32_t get_var_i_fuel_ml() {
    return i_fuel_ml;
}

void set_var_i_fuel_ml(int32_t value) {
    i_fuel_ml = value;
}

const char *get_var_s_pump_pwr_manu() {
    return s_pump_pwr_manu;
}

void set_var_s_pump_pwr_manu(const char *value) {
    strncpy(s_pump_pwr_manu, value, sizeof(s_pump_pwr_manu) / sizeof(char));
    s_pump_pwr_manu[sizeof(s_pump_pwr_manu) / sizeof(char) - 1] = 0;
}

const char *get_var_s_pump_pwr_calib() {
    return s_pump_pwr_calib;
}

void set_var_s_pump_pwr_calib(const char *value) {
    strncpy(s_pump_pwr_calib, value, sizeof(s_pump_pwr_calib) / sizeof(char));
    s_pump_pwr_calib[sizeof(s_pump_pwr_calib) / sizeof(char) - 1] = 0;
}

const char *get_var_s_sys_power_off_time() {
    return s_sys_power_off_time;
}

void set_var_s_sys_power_off_time(const char *value) {
    strncpy(s_sys_power_off_time, value, sizeof(s_sys_power_off_time) / sizeof(char));
    s_sys_power_off_time[sizeof(s_sys_power_off_time) / sizeof(char) - 1] = 0;
}

const char *get_var_s_modelname() {
    return s_modelname;
}

void set_var_s_modelname(const char *value) {
    strncpy(s_modelname, value, sizeof(s_modelname) / sizeof(char));
    s_modelname[sizeof(s_modelname) / sizeof(char) - 1] = 0;
}

const char *get_var_s_tank_type() {
    return s_tank_type;
}

void set_var_s_tank_type(const char *value) {
    strncpy(s_tank_type, value, sizeof(s_tank_type) / sizeof(char));
    s_tank_type[sizeof(s_tank_type) / sizeof(char) - 1] = 0;
}

bool get_var_b_pump_pwr_disabled() {
    return b_pump_pwr_disabled;
}

void set_var_b_pump_pwr_disabled(bool value) {
    b_pump_pwr_disabled = value;
}

int32_t get_var_i_tank_type_model() {
    return i_tank_type_model;
}

void set_var_i_tank_type_model(int32_t value) {
    i_tank_type_model = value;
}

const char *get_var_s_pump_pwr_model() {
    return s_pump_pwr_model;
}

void set_var_s_pump_pwr_model(const char *value) {
    strncpy(s_pump_pwr_model, value, sizeof(s_pump_pwr_model) / sizeof(char));
    s_pump_pwr_model[sizeof(s_pump_pwr_model) / sizeof(char) - 1] = 0;
}

const char *get_var_s_fueling_total() {
    return s_fueling_total;
}

void set_var_s_fueling_total(const char *value) {
    strncpy(s_fueling_total, value, sizeof(s_fueling_total) / sizeof(char));
    s_fueling_total[sizeof(s_fueling_total) / sizeof(char) - 1] = 0;
}

const char *get_var_s_fueling_count() {
    return s_fueling_count;
}

void set_var_s_fueling_count(const char *value) {
    strncpy(s_fueling_count, value, sizeof(s_fueling_count) / sizeof(char));
    s_fueling_count[sizeof(s_fueling_count) / sizeof(char) - 1] = 0;
}

const char *get_var_s_fueling_last() {
    return s_fueling_last;
}

void set_var_s_fueling_last(const char *value) {
    strncpy(s_fueling_last, value, sizeof(s_fueling_last) / sizeof(char));
    s_fueling_last[sizeof(s_fueling_last) / sizeof(char) - 1] = 0;
}

const char *get_var_s_max_refuel_time() {
    return s_max_refuel_time;
}

void set_var_s_max_refuel_time(const char *value) {
    strncpy(s_max_refuel_time, value, sizeof(s_max_refuel_time) / sizeof(char));
    s_max_refuel_time[sizeof(s_max_refuel_time) / sizeof(char) - 1] = 0;
}

const char *get_var_s_max_defuel_time() {
    return s_max_defuel_time;
}

void set_var_s_max_defuel_time(const char *value) {
    strncpy(s_max_defuel_time, value, sizeof(s_max_defuel_time) / sizeof(char));
    s_max_defuel_time[sizeof(s_max_defuel_time) / sizeof(char) - 1] = 0;
}

const char *get_var_s_back_fuel_time() {
    return s_back_fuel_time;
}

void set_var_s_back_fuel_time(const char *value) {
    strncpy(s_back_fuel_time, value, sizeof(s_back_fuel_time) / sizeof(char));
    s_back_fuel_time[sizeof(s_back_fuel_time) / sizeof(char) - 1] = 0;
}

const char *get_var_s_air_removal_time() {
    return s_air_removal_time;
}

void set_var_s_air_removal_time(const char *value) {
    strncpy(s_air_removal_time, value, sizeof(s_air_removal_time) / sizeof(char));
    s_air_removal_time[sizeof(s_air_removal_time) / sizeof(char) - 1] = 0;
}

const char *get_var_s_pump_stop_empty_delay() {
    return s_pump_stop_empty_delay;
}

void set_var_s_pump_stop_empty_delay(const char *value) {
    strncpy(s_pump_stop_empty_delay, value, sizeof(s_pump_stop_empty_delay) / sizeof(char));
    s_pump_stop_empty_delay[sizeof(s_pump_stop_empty_delay) / sizeof(char) - 1] = 0;
}

const char *get_var_s_messurement_delay() {
    return s_messurement_delay;
}

void set_var_s_messurement_delay(const char *value) {
    strncpy(s_messurement_delay, value, sizeof(s_messurement_delay) / sizeof(char));
    s_messurement_delay[sizeof(s_messurement_delay) / sizeof(char) - 1] = 0;
}

const char *get_var_s_max_defuel_ml() {
    return s_max_defuel_ml;
}

void set_var_s_max_defuel_ml(const char *value) {
    strncpy(s_max_defuel_ml, value, sizeof(s_max_defuel_ml) / sizeof(char));
    s_max_defuel_ml[sizeof(s_max_defuel_ml) / sizeof(char) - 1] = 0;
}

const char *get_var_s_max_pressure() {
    return s_max_pressure;
}

void set_var_s_max_pressure(const char *value) {
    strncpy(s_max_pressure, value, sizeof(s_max_pressure) / sizeof(char));
    s_max_pressure[sizeof(s_max_pressure) / sizeof(char) - 1] = 0;
}

const char *get_var_s_pump_stop_pressure_empty() {
    return s_pump_stop_pressure_empty;
}

void set_var_s_pump_stop_pressure_empty(const char *value) {
    strncpy(s_pump_stop_pressure_empty, value, sizeof(s_pump_stop_pressure_empty) / sizeof(char));
    s_pump_stop_pressure_empty[sizeof(s_pump_stop_pressure_empty) / sizeof(char) - 1] = 0;
}

const char *get_var_s_hopper_pressure() {
    return s_hopper_pressure;
}

void set_var_s_hopper_pressure(const char *value) {
    strncpy(s_hopper_pressure, value, sizeof(s_hopper_pressure) / sizeof(char));
    s_hopper_pressure[sizeof(s_hopper_pressure) / sizeof(char) - 1] = 0;
}

const char *get_var_s_pump_stop_hopper_pressure_diff() {
    return s_pump_stop_hopper_pressure_diff;
}

void set_var_s_pump_stop_hopper_pressure_diff(const char *value) {
    strncpy(s_pump_stop_hopper_pressure_diff, value, sizeof(s_pump_stop_hopper_pressure_diff) / sizeof(char));
    s_pump_stop_hopper_pressure_diff[sizeof(s_pump_stop_hopper_pressure_diff) / sizeof(char) - 1] = 0;
}

const char *get_var_s_pressure_drop_hose_break() {
    return s_pressure_drop_hose_break;
}

void set_var_s_pressure_drop_hose_break(const char *value) {
    strncpy(s_pressure_drop_hose_break, value, sizeof(s_pressure_drop_hose_break) / sizeof(char));
    s_pressure_drop_hose_break[sizeof(s_pressure_drop_hose_break) / sizeof(char) - 1] = 0;
}

const char *get_var_s_pump_stop_pressure_diff() {
    return s_pump_stop_pressure_diff;
}

void set_var_s_pump_stop_pressure_diff(const char *value) {
    strncpy(s_pump_stop_pressure_diff, value, sizeof(s_pump_stop_pressure_diff) / sizeof(char));
    s_pump_stop_pressure_diff[sizeof(s_pump_stop_pressure_diff) / sizeof(char) - 1] = 0;
}

const char *get_var_s_max_refuel_ml() {
    return s_max_refuel_ml;
}

void set_var_s_max_refuel_ml(const char *value) {
    strncpy(s_max_refuel_ml, value, sizeof(s_max_refuel_ml) / sizeof(char));
    s_max_refuel_ml[sizeof(s_max_refuel_ml) / sizeof(char) - 1] = 0;
}

bool get_var_b_hide_dialog_save_system() {
    return b_hide_dialog_save_system;
}

void set_var_b_hide_dialog_save_system(bool value) {
    b_hide_dialog_save_system = value;
}

bool get_var_b_disable_btn_settings() {
    return b_disable_btn_settings;
}

void set_var_b_disable_btn_settings(bool value) {
    b_disable_btn_settings = value;
}

bool get_var_b_hide_dialog_save_model() {
    return b_hide_dialog_save_model;
}

void set_var_b_hide_dialog_save_model(bool value) {
    b_hide_dialog_save_model = value;
}

bool get_var_b_disable_btn_home() {
    return b_disable_btn_home;
}

void set_var_b_disable_btn_home(bool value) {
    b_disable_btn_home = value;
}

const char *get_var_s_fuel_ml() {
    return s_fuel_ml;
}

void set_var_s_fuel_ml(const char *value) {
    strncpy(s_fuel_ml, value, sizeof(s_fuel_ml) / sizeof(char));
    s_fuel_ml[sizeof(s_fuel_ml) / sizeof(char) - 1] = 0;
}

const char *get_var_s_cut_off_ml() {
    return s_cut_off_ml;
}

void set_var_s_cut_off_ml(const char *value) {
    strncpy(s_cut_off_ml, value, sizeof(s_cut_off_ml) / sizeof(char));
    s_cut_off_ml[sizeof(s_cut_off_ml) / sizeof(char) - 1] = 0;
}

const char *get_var_s_fuel_mbar() {
    return s_fuel_mbar;
}

void set_var_s_fuel_mbar(const char *value) {
    strncpy(s_fuel_mbar, value, sizeof(s_fuel_mbar) / sizeof(char));
    s_fuel_mbar[sizeof(s_fuel_mbar) / sizeof(char) - 1] = 0;
}

const char *get_var_s_fuel_diff_mbar() {
    return s_fuel_diff_mbar;
}

void set_var_s_fuel_diff_mbar(const char *value) {
    strncpy(s_fuel_diff_mbar, value, sizeof(s_fuel_diff_mbar) / sizeof(char));
    s_fuel_diff_mbar[sizeof(s_fuel_diff_mbar) / sizeof(char) - 1] = 0;
}


/* Modellparameter ausblenden */

bool get_var_b_hide_mod_menge() {
    return b_hide_mod_menge;
}

void set_var_b_hide_mod_menge(bool value) {
    b_hide_mod_menge = value;
}

bool get_var_b_hide_mod_pump_pwr() {
    return b_hide_mod_pump_pwr;
}

void set_var_b_hide_mod_pump_pwr(bool value) {
    b_hide_mod_pump_pwr = value;
}

bool get_var_b_hide_mod_drop_hose_break() {
    return b_hide_mod_drop_hose_break;
}

void set_var_b_hide_mod_drop_hose_break(bool value) {
    b_hide_mod_drop_hose_break = value;
}

bool get_var_b_hide_mod_max_refuel_time() {
    return b_hide_mod_max_refuel_time;
}

void set_var_b_hide_mod_max_refuel_time(bool value) {
    b_hide_mod_max_refuel_time = value;
}

bool get_var_b_hide_mod_max_defuel_time() {
    return b_hide_mod_max_defuel_time;
}

void set_var_b_hide_mod_max_defuel_time(bool value) {
    b_hide_mod_max_defuel_time = value;
}

bool get_var_b_hide_mod_back_fuel_time() {
    return b_hide_mod_back_fuel_time;
}

void set_var_b_hide_mod_back_fuel_time(bool value) {
    b_hide_mod_back_fuel_time = value;
}

bool get_var_b_hide_mod_air_removal_time() {
    return b_hide_mod_air_removal_time;
}

void set_var_b_hide_mod_air_removal_time(bool value) {
    b_hide_mod_air_removal_time = value;
}

bool get_var_b_hide_mod_pump_stop_empty_delay() {
    return b_hide_mod_pump_stop_empty_delay;
}

void set_var_b_hide_mod_pump_stop_empty_delay(bool value) {
    b_hide_mod_pump_stop_empty_delay = value;
}

bool get_var_b_hide_mod_messurement_delay() {
    return b_hide_mod_messurement_delay;
}

void set_var_b_hide_mod_messurement_delay(bool value) {
    b_hide_mod_messurement_delay = value;
}

bool get_var_b_hide_mod_max_refuel_ml() {
    return b_hide_mod_max_refuel_ml;
}

void set_var_b_hide_mod_max_refuel_ml(bool value) {
    b_hide_mod_max_refuel_ml = value;
}

bool get_var_b_hide_mod_max_defuel_ml() {
    return b_hide_mod_max_defuel_ml;
}

void set_var_b_hide_mod_max_defuel_ml(bool value) {
    b_hide_mod_max_defuel_ml = value;
}

bool get_var_b_hide_mod_max_pressure() {
    return b_hide_mod_max_pressure;
}

void set_var_b_hide_mod_max_pressure(bool value) {
    b_hide_mod_max_pressure = value;
}

bool get_var_b_hide_mod_pump_stop_pressure_diff() {
    return b_hide_mod_pump_stop_pressure_diff;
}

void set_var_b_hide_mod_pump_stop_pressure_diff(bool value) {
    b_hide_mod_pump_stop_pressure_diff = value;
}

bool get_var_b_hide_mod_pump_stop_pressure_empty() {
    return b_hide_mod_pump_stop_pressure_empty;
}

void set_var_b_hide_mod_pump_stop_pressure_empty(bool value) {
    b_hide_mod_pump_stop_pressure_empty = value;
}

bool get_var_b_hide_mod_hopper_pressure() {
    return b_hide_mod_hopper_pressure;
}

void set_var_b_hide_mod_hopper_pressure(bool value) {
    b_hide_mod_hopper_pressure = value;
}

bool get_var_b_hide_mod_pump_stop_hopper_pressure_diff() {
    return b_hide_mod_pump_stop_hopper_pressure_diff;
}

void set_var_b_hide_mod_pump_stop_hopper_pressure_diff(bool value) {
    b_hide_mod_pump_stop_hopper_pressure_diff = value;
}

bool get_var_b_hide_mod_fueling_last() {
    return b_hide_mod_fueling_last;
}

void set_var_b_hide_mod_fueling_last(bool value) {
    b_hide_mod_fueling_last = value;
}

bool get_var_b_hide_mod_fueling_count() {
    return b_hide_mod_fueling_count;
}

void set_var_b_hide_mod_fueling_count(bool value) {
    b_hide_mod_fueling_count = value;
}

bool get_var_b_hide_mod_fueling_total() {
    return b_hide_mod_fueling_total;
}

void set_var_b_hide_mod_fueling_total(bool value) {
    b_hide_mod_fueling_total = value;
}

bool get_var_b_hide_mod_bag_info() {
    return b_hide_mod_bag_info;
}

void set_var_b_hide_mod_bag_info(bool value) {
    b_hide_mod_bag_info = value;
}

bool get_var_b_hide_pump_save_mod_to_controller() {
    return b_hide_pump_save_mod_to_controller;
}

void set_var_b_hide_pump_save_mod_to_controller(bool value) {
    b_hide_pump_save_mod_to_controller = value;
}

const char *get_var_s_akku_volt() {
    return s_akku_volt;
}

void set_var_s_akku_volt(const char *value) {
    strncpy(s_akku_volt, value, sizeof(s_akku_volt) / sizeof(char));
    s_akku_volt[sizeof(s_akku_volt) / sizeof(char) - 1] = 0;
}

bool get_var_b_hide_box_start_error() {
    return b_hide_box_start_error;
}

void set_var_b_hide_box_start_error(bool value) {
    b_hide_box_start_error = value;
}

bool get_var_b_hide_wait() {
    return b_hide_wait;
}

void set_var_b_hide_wait(bool value) {
    b_hide_wait = value;
}