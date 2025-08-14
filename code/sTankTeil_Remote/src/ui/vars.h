#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_S_SCREEN_TITEL = 0,
    FLOW_GLOBAL_VARIABLE_S_OWNER = 1,
    FLOW_GLOBAL_VARIABLE_S_FLOW_TICKS = 2,
    FLOW_GLOBAL_VARIABLE_S_PRESSURE_AVG = 3,
    FLOW_GLOBAL_VARIABLE_S_AKKU_MIN_V = 4,
    FLOW_GLOBAL_VARIABLE_B_LOAD_LAST_MODEL = 5,
    FLOW_GLOBAL_VARIABLE_B_SIGNAL = 6,
    FLOW_GLOBAL_VARIABLE_B_HIDE_KEYBOARD = 7,
    FLOW_GLOBAL_VARIABLE_B_HIDE_NUMPAD = 8,
    FLOW_GLOBAL_VARIABLE_I_PUMP_PWR = 9,
    FLOW_GLOBAL_VARIABLE_S_PUMP_PWR = 10,
    FLOW_GLOBAL_VARIABLE_B_HIDE_CONT_FLOW_CALIBRATE = 11,
    FLOW_GLOBAL_VARIABLE_B_HIDE_CONT_MODEL_FUEL = 12,
    FLOW_GLOBAL_VARIABLE_I_CALIB_FLOW_SENSOR = 13,
    FLOW_GLOBAL_VARIABLE_B_IS_PUMPING = 14,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MODEL_FUEL = 15,
    FLOW_GLOBAL_VARIABLE_S_STATUS = 16,
    FLOW_GLOBAL_VARIABLE_I_FUEL_ML = 17,
    FLOW_GLOBAL_VARIABLE_S_PUMP_PWR_MANU = 18,
    FLOW_GLOBAL_VARIABLE_S_PUMP_PWR_CALIB = 19,
    FLOW_GLOBAL_VARIABLE_S_SYS_POWER_OFF_TIME = 20,
    FLOW_GLOBAL_VARIABLE_S_MODELNAME = 21,
    FLOW_GLOBAL_VARIABLE_B_PUMP_PWR_DISABLED = 22,
    FLOW_GLOBAL_VARIABLE_S_PUMP_PWR_MODEL = 23,
    FLOW_GLOBAL_VARIABLE_S_TANK_TYPE = 24,
    FLOW_GLOBAL_VARIABLE_I_TANK_TYPE_MODEL = 25,
    FLOW_GLOBAL_VARIABLE_S_FUELING_TOTAL = 26,
    FLOW_GLOBAL_VARIABLE_S_FUELING_COUNT = 27,
    FLOW_GLOBAL_VARIABLE_S_FUELING_LAST = 28,
    FLOW_GLOBAL_VARIABLE_S_MAX_REFUEL_TIME = 29,
    FLOW_GLOBAL_VARIABLE_S_MAX_DEFUEL_TIME = 30,
    FLOW_GLOBAL_VARIABLE_S_BACK_FUEL_TIME = 31,
    FLOW_GLOBAL_VARIABLE_S_AIR_REMOVAL_TIME = 32,
    FLOW_GLOBAL_VARIABLE_S_PUMP_STOP_EMPTY_DELAY = 33,
    FLOW_GLOBAL_VARIABLE_S_MESSUREMENT_DELAY = 34,
    FLOW_GLOBAL_VARIABLE_S_MAX_REFUEL_ML = 35,
    FLOW_GLOBAL_VARIABLE_S_MAX_DEFUEL_ML = 36,
    FLOW_GLOBAL_VARIABLE_S_MAX_PRESSURE = 37,
    FLOW_GLOBAL_VARIABLE_S_PUMP_STOP_PRESSURE_DIFF = 38,
    FLOW_GLOBAL_VARIABLE_S_PUMP_STOP_PRESSURE_EMPTY = 39,
    FLOW_GLOBAL_VARIABLE_S_HOPPER_PRESSURE = 40,
    FLOW_GLOBAL_VARIABLE_S_PUMP_STOP_HOPPER_PRESSURE_DIFF = 41,
    FLOW_GLOBAL_VARIABLE_S_PRESSURE_DROP_HOSE_BREAK = 42,
    FLOW_GLOBAL_VARIABLE_B_HIDE_DIALOG_SAVE_SYSTEM = 43,
    FLOW_GLOBAL_VARIABLE_B_DISABLE_BTN_SETTINGS = 44,
    FLOW_GLOBAL_VARIABLE_B_HIDE_DIALOG_SAVE_MODEL = 45,
    FLOW_GLOBAL_VARIABLE_B_DISABLE_BTN_HOME = 46,
    FLOW_GLOBAL_VARIABLE_S_FUEL_ML = 47,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MANUEL_FUEL = 48,
    FLOW_GLOBAL_VARIABLE_S_CUT_OFF_ML = 49,
    FLOW_GLOBAL_VARIABLE_S_FUEL_MBAR = 50,
    FLOW_GLOBAL_VARIABLE_S_FUEL_DIFF_MBAR = 51,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_MENGE = 52,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_PUMP_PWR = 53,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_DROP_HOSE_BREAK = 54,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_MAX_REFUEL_TIME = 55,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_MAX_DEFUEL_TIME = 56,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_BACK_FUEL_TIME = 57,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_AIR_REMOVAL_TIME = 58,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_PUMP_STOP_EMPTY_DELAY = 59,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_MESSUREMENT_DELAY = 60,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_MAX_REFUEL_ML = 61,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_MAX_DEFUEL_ML = 62,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_MAX_PRESSURE = 63,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_PUMP_STOP_PRESSURE_DIFF = 64,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_PUMP_STOP_PRESSURE_EMPTY = 65,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_HOPPER_PRESSURE = 66,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_PUMP_STOP_HOPPER_PRESSURE_DIFF = 67,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_FUELING_LAST = 68,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_FUELING_COUNT = 69,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_FUELING_TOTAL = 70,
    FLOW_GLOBAL_VARIABLE_B_HIDE_MOD_BAG_INFO = 71,
    FLOW_GLOBAL_VARIABLE_B_HIDE_PUMP_SAVE_MOD_TO_CONTROLLER = 72,
    FLOW_GLOBAL_VARIABLE_S_AKKU_VOLT = 73,
    FLOW_GLOBAL_VARIABLE_B_HIDE_BOX_START_ERROR = 74,
    FLOW_GLOBAL_VARIABLE_B_HIDE_WAIT = 75
};

// Native global variables

extern const char *get_var_s_screen_titel();
extern void set_var_s_screen_titel(const char *value);
extern const char *get_var_s_owner();
extern void set_var_s_owner(const char *value);
extern const char *get_var_s_flow_ticks();
extern void set_var_s_flow_ticks(const char *value);
extern const char *get_var_s_pressure_avg();
extern void set_var_s_pressure_avg(const char *value);
extern const char *get_var_s_akku_min_v();
extern void set_var_s_akku_min_v(const char *value);
extern bool get_var_b_load_last_model();
extern void set_var_b_load_last_model(bool value);
extern bool get_var_b_signal();
extern void set_var_b_signal(bool value);
extern bool get_var_b_hide_keyboard();
extern void set_var_b_hide_keyboard(bool value);
extern bool get_var_b_hide_numpad();
extern void set_var_b_hide_numpad(bool value);
extern int32_t get_var_i_pump_pwr();
extern void set_var_i_pump_pwr(int32_t value);
extern const char *get_var_s_pump_pwr();
extern void set_var_s_pump_pwr(const char *value);
extern bool get_var_b_hide_cont_flow_calibrate();
extern void set_var_b_hide_cont_flow_calibrate(bool value);
extern bool get_var_b_hide_cont_model_fuel();
extern void set_var_b_hide_cont_model_fuel(bool value);
extern const char *get_var_i_calib_flow_sensor();
extern void set_var_i_calib_flow_sensor(const char *value);
extern bool get_var_b_is_pumping();
extern void set_var_b_is_pumping(bool value);
extern bool get_var_b_hide_model_fuel();
extern void set_var_b_hide_model_fuel(bool value);
extern const char *get_var_s_status();
extern void set_var_s_status(const char *value);
extern int32_t get_var_i_fuel_ml();
extern void set_var_i_fuel_ml(int32_t value);
extern const char *get_var_s_pump_pwr_manu();
extern void set_var_s_pump_pwr_manu(const char *value);
extern const char *get_var_s_pump_pwr_calib();
extern void set_var_s_pump_pwr_calib(const char *value);
extern const char *get_var_s_sys_power_off_time();
extern void set_var_s_sys_power_off_time(const char *value);
extern const char *get_var_s_modelname();
extern void set_var_s_modelname(const char *value);
extern bool get_var_b_pump_pwr_disabled();
extern void set_var_b_pump_pwr_disabled(bool value);
extern const char *get_var_s_pump_pwr_model();
extern void set_var_s_pump_pwr_model(const char *value);
extern const char *get_var_s_tank_type();
extern void set_var_s_tank_type(const char *value);
extern int32_t get_var_i_tank_type_model();
extern void set_var_i_tank_type_model(int32_t value);
extern const char *get_var_s_fueling_total();
extern void set_var_s_fueling_total(const char *value);
extern const char *get_var_s_fueling_count();
extern void set_var_s_fueling_count(const char *value);
extern const char *get_var_s_fueling_last();
extern void set_var_s_fueling_last(const char *value);
extern const char *get_var_s_max_refuel_time();
extern void set_var_s_max_refuel_time(const char *value);
extern const char *get_var_s_max_defuel_time();
extern void set_var_s_max_defuel_time(const char *value);
extern const char *get_var_s_back_fuel_time();
extern void set_var_s_back_fuel_time(const char *value);
extern const char *get_var_s_air_removal_time();
extern void set_var_s_air_removal_time(const char *value);
extern const char *get_var_s_pump_stop_empty_delay();
extern void set_var_s_pump_stop_empty_delay(const char *value);
extern const char *get_var_s_messurement_delay();
extern void set_var_s_messurement_delay(const char *value);
extern const char *get_var_s_max_refuel_ml();
extern void set_var_s_max_refuel_ml(const char *value);
extern const char *get_var_s_max_defuel_ml();
extern void set_var_s_max_defuel_ml(const char *value);
extern const char *get_var_s_max_pressure();
extern void set_var_s_max_pressure(const char *value);
extern const char *get_var_s_pump_stop_pressure_diff();
extern void set_var_s_pump_stop_pressure_diff(const char *value);
extern const char *get_var_s_pump_stop_pressure_empty();
extern void set_var_s_pump_stop_pressure_empty(const char *value);
extern const char *get_var_s_hopper_pressure();
extern void set_var_s_hopper_pressure(const char *value);
extern const char *get_var_s_pump_stop_hopper_pressure_diff();
extern void set_var_s_pump_stop_hopper_pressure_diff(const char *value);
extern const char *get_var_s_pressure_drop_hose_break();
extern void set_var_s_pressure_drop_hose_break(const char *value);
extern bool get_var_b_hide_dialog_save_system();
extern void set_var_b_hide_dialog_save_system(bool value);
extern bool get_var_b_disable_btn_settings();
extern void set_var_b_disable_btn_settings(bool value);
extern bool get_var_b_hide_dialog_save_model();
extern void set_var_b_hide_dialog_save_model(bool value);
extern bool get_var_b_disable_btn_home();
extern void set_var_b_disable_btn_home(bool value);
extern const char *get_var_s_fuel_ml();
extern void set_var_s_fuel_ml(const char *value);
extern bool get_var_b_hide_manuel_fuel();
extern void set_var_b_hide_manuel_fuel(bool value);
extern const char *get_var_s_cut_off_ml();
extern void set_var_s_cut_off_ml(const char *value);
extern const char *get_var_s_fuel_mbar();
extern void set_var_s_fuel_mbar(const char *value);
extern const char *get_var_s_fuel_diff_mbar();
extern void set_var_s_fuel_diff_mbar(const char *value);
extern bool get_var_b_hide_mod_menge();
extern void set_var_b_hide_mod_menge(bool value);
extern bool get_var_b_hide_mod_pump_pwr();
extern void set_var_b_hide_mod_pump_pwr(bool value);
extern bool get_var_b_hide_mod_drop_hose_break();
extern void set_var_b_hide_mod_drop_hose_break(bool value);
extern bool get_var_b_hide_mod_max_refuel_time();
extern void set_var_b_hide_mod_max_refuel_time(bool value);
extern bool get_var_b_hide_mod_max_defuel_time();
extern void set_var_b_hide_mod_max_defuel_time(bool value);
extern bool get_var_b_hide_mod_back_fuel_time();
extern void set_var_b_hide_mod_back_fuel_time(bool value);
extern bool get_var_b_hide_mod_air_removal_time();
extern void set_var_b_hide_mod_air_removal_time(bool value);
extern bool get_var_b_hide_mod_pump_stop_empty_delay();
extern void set_var_b_hide_mod_pump_stop_empty_delay(bool value);
extern bool get_var_b_hide_mod_messurement_delay();
extern void set_var_b_hide_mod_messurement_delay(bool value);
extern bool get_var_b_hide_mod_max_refuel_ml();
extern void set_var_b_hide_mod_max_refuel_ml(bool value);
extern bool get_var_b_hide_mod_max_defuel_ml();
extern void set_var_b_hide_mod_max_defuel_ml(bool value);
extern bool get_var_b_hide_mod_max_pressure();
extern void set_var_b_hide_mod_max_pressure(bool value);
extern bool get_var_b_hide_mod_pump_stop_pressure_diff();
extern void set_var_b_hide_mod_pump_stop_pressure_diff(bool value);
extern bool get_var_b_hide_mod_pump_stop_pressure_empty();
extern void set_var_b_hide_mod_pump_stop_pressure_empty(bool value);
extern bool get_var_b_hide_mod_hopper_pressure();
extern void set_var_b_hide_mod_hopper_pressure(bool value);
extern bool get_var_b_hide_mod_pump_stop_hopper_pressure_diff();
extern void set_var_b_hide_mod_pump_stop_hopper_pressure_diff(bool value);
extern bool get_var_b_hide_mod_fueling_last();
extern void set_var_b_hide_mod_fueling_last(bool value);
extern bool get_var_b_hide_mod_fueling_count();
extern void set_var_b_hide_mod_fueling_count(bool value);
extern bool get_var_b_hide_mod_fueling_total();
extern void set_var_b_hide_mod_fueling_total(bool value);
extern bool get_var_b_hide_mod_bag_info();
extern void set_var_b_hide_mod_bag_info(bool value);
extern bool get_var_b_hide_pump_save_mod_to_controller();
extern void set_var_b_hide_pump_save_mod_to_controller(bool value);
extern const char *get_var_s_akku_volt();
extern void set_var_s_akku_volt(const char *value);
extern bool get_var_b_hide_box_start_error();
extern void set_var_b_hide_box_start_error(bool value);
extern bool get_var_b_hide_wait();
extern void set_var_b_hide_wait(bool value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/