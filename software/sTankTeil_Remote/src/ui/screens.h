#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *settings_system;
    lv_obj_t *settings;
    lv_obj_t *model_select;
    lv_obj_t *main;
    lv_obj_t *settings_model;
    lv_obj_t *pump;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *obj3__btn_home;
    lv_obj_t *obj3__btn_settings;
    lv_obj_t *obj3__obj0;
    lv_obj_t *obj3__lbl_screen_titel;
    lv_obj_t *obj3__lbl_batt;
    lv_obj_t *cont_sys_settings;
    lv_obj_t *txt_owner;
    lv_obj_t *txt_sys_power_off_time;
    lv_obj_t *txt_flow_ticks;
    lv_obj_t *txt_pressure_avg;
    lv_obj_t *txt_akku_min_v;
    lv_obj_t *txt_pump_pwr_calib;
    lv_obj_t *txt_pump_pwr_manu;
    lv_obj_t *cont_keyboard_settings;
    lv_obj_t *cont_keyboard_settings__kb_keyboard;
    lv_obj_t *cont_numpad_settings;
    lv_obj_t *cont_numpad_settings__kb_numpad;
    lv_obj_t *obj4;
    lv_obj_t *obj4__wnd_save_system_settings;
    lv_obj_t *cont_calib_volt_settings;
    lv_obj_t *cont_calib_volt_settings__wnd_calibrate_volt;
    lv_obj_t *cont_calib_volt_settings__txt_calib_volt;
    lv_obj_t *cont_calib_volt_settings__obj2;
    lv_obj_t *obj5;
    lv_obj_t *obj5__btn_home;
    lv_obj_t *obj5__btn_settings;
    lv_obj_t *obj5__obj0;
    lv_obj_t *obj5__lbl_screen_titel;
    lv_obj_t *obj5__lbl_batt;
    lv_obj_t *obj6;
    lv_obj_t *obj6__btn_home;
    lv_obj_t *obj6__btn_settings;
    lv_obj_t *obj6__obj0;
    lv_obj_t *obj6__lbl_screen_titel;
    lv_obj_t *obj6__lbl_batt;
    lv_obj_t *cont_models;
    lv_obj_t *lbl_model_plus;
    lv_obj_t *obj7;
    lv_obj_t *obj7__obj1;
    lv_obj_t *logo;
    lv_obj_t *obj8;
    lv_obj_t *obj9;
    lv_obj_t *obj10;
    lv_obj_t *obj11;
    lv_obj_t *obj12;
    lv_obj_t *obj13;
    lv_obj_t *obj14;
    lv_obj_t *obj14__btn_home;
    lv_obj_t *obj14__btn_settings;
    lv_obj_t *obj14__obj0;
    lv_obj_t *obj14__lbl_screen_titel;
    lv_obj_t *obj14__lbl_batt;
    lv_obj_t *cont_model_settings;
    lv_obj_t *obj15;
    lv_obj_t *obj16;
    lv_obj_t *obj17;
    lv_obj_t *obj18;
    lv_obj_t *obj19;
    lv_obj_t *obj20;
    lv_obj_t *obj21;
    lv_obj_t *obj22;
    lv_obj_t *obj23;
    lv_obj_t *obj24;
    lv_obj_t *obj25;
    lv_obj_t *obj26;
    lv_obj_t *obj27;
    lv_obj_t *obj28;
    lv_obj_t *obj29;
    lv_obj_t *obj30;
    lv_obj_t *obj31;
    lv_obj_t *obj32;
    lv_obj_t *obj33;
    lv_obj_t *obj34;
    lv_obj_t *cont_keyboard_model;
    lv_obj_t *cont_keyboard_model__kb_keyboard;
    lv_obj_t *cont_numpad_model;
    lv_obj_t *cont_numpad_model__kb_numpad;
    lv_obj_t *obj35;
    lv_obj_t *obj35__wnd_save_system_settings;
    lv_obj_t *obj36;
    lv_obj_t *obj37;
    lv_obj_t *obj38;
    lv_obj_t *obj39;
    lv_obj_t *obj40;
    lv_obj_t *obj41;
    lv_obj_t *obj42;
    lv_obj_t *obj43;
    lv_obj_t *obj44;
    lv_obj_t *obj45;
    lv_obj_t *obj46;
    lv_obj_t *obj47;
    lv_obj_t *obj48;
    lv_obj_t *obj49;
    lv_obj_t *obj50;
    lv_obj_t *obj51;
    lv_obj_t *obj52;
    lv_obj_t *obj53;
    lv_obj_t *obj54;
    lv_obj_t *obj55;
    lv_obj_t *obj56;
    lv_obj_t *obj57;
    lv_obj_t *obj58;
    lv_obj_t *obj58__btn_home;
    lv_obj_t *obj58__btn_settings;
    lv_obj_t *obj58__obj0;
    lv_obj_t *obj58__lbl_screen_titel;
    lv_obj_t *obj58__lbl_batt;
    lv_obj_t *cont_pump_main;
    lv_obj_t *obj59;
    lv_obj_t *cont_flow_calibrate;
    lv_obj_t *obj60;
    lv_obj_t *obj61;
    lv_obj_t *cont_model_calibrate;
    lv_obj_t *cont_model_fuel;
    lv_obj_t *bar_ml;
    lv_obj_t *scale_ml;
    lv_obj_t *cont_manuel_fuel;
    lv_obj_t *obj62;
    lv_obj_t *obj63;
    lv_obj_t *obj64;
    lv_obj_t *obj65;
    lv_obj_t *obj66;
    lv_obj_t *obj67;
    lv_obj_t *obj68;
    lv_obj_t *obj69;
    lv_obj_t *obj70;
    lv_obj_t *obj71;
    lv_obj_t *obj72;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_SETTINGS_SYSTEM = 1,
    SCREEN_ID_SETTINGS = 2,
    SCREEN_ID_MODEL_SELECT = 3,
    SCREEN_ID_MAIN = 4,
    SCREEN_ID_SETTINGS_MODEL = 5,
    SCREEN_ID_PUMP = 6,
};

void create_screen_settings_system();
void tick_screen_settings_system();

void create_screen_settings();
void tick_screen_settings();

void create_screen_model_select();
void tick_screen_model_select();

void create_screen_main();
void tick_screen_main();

void create_screen_settings_model();
void tick_screen_settings_model();

void create_screen_pump();
void tick_screen_pump();

void create_user_widget_background(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_background(int startWidgetIndex);

void create_user_widget_header(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_header(int startWidgetIndex);

void create_user_widget_keyboard(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_keyboard(int startWidgetIndex);

void create_user_widget_numpad(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_numpad(int startWidgetIndex);

void create_user_widget_save(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_save(int startWidgetIndex);

void create_user_widget_wait(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_wait(int startWidgetIndex);

void create_user_widget_calib_volt(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_calib_volt(int startWidgetIndex);

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/