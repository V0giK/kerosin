#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;

static void event_handler_cb_settings_system_txt_owner(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_owner(value);
        }
    }
}

static void event_handler_cb_settings_system_obj0(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_b_load_last_model(value);
        }
    }
}

static void event_handler_cb_settings_system_obj1(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_b_signal(value);
        }
    }
}

static void event_handler_cb_settings_system_txt_sys_power_off_time(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_sys_power_off_time(value);
        }
    }
}

static void event_handler_cb_settings_system_txt_flow_ticks(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_flow_ticks(value);
        }
    }
}

static void event_handler_cb_settings_system_txt_pressure_avg(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_pressure_avg(value);
        }
    }
}

static void event_handler_cb_settings_system_txt_akku_min_v(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_akku_min_v(value);
        }
    }
}

static void event_handler_cb_settings_system_txt_pump_pwr_calib(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_pump_pwr_calib(value);
        }
    }
}

static void event_handler_cb_settings_system_txt_pump_pwr_manu(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_pump_pwr_manu(value);
        }
    }
}

static void event_handler_cb_settings_model_obj2(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_modelname(value);
        }
    }
}

static void event_handler_cb_settings_model_obj3(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_dropdown_get_selected(ta);
            set_var_i_tank_type_model(value);
        }
    }
}

static void event_handler_cb_settings_model_obj4(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_pump_pwr_model(value);
        }
    }
}

static void event_handler_cb_settings_model_obj5(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_pressure_drop_hose_break(value);
        }
    }
}

static void event_handler_cb_settings_model_obj6(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_max_refuel_time(value);
        }
    }
}

static void event_handler_cb_settings_model_obj7(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_max_defuel_time(value);
        }
    }
}

static void event_handler_cb_settings_model_obj8(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_back_fuel_time(value);
        }
    }
}

static void event_handler_cb_settings_model_obj9(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_air_removal_time(value);
        }
    }
}

static void event_handler_cb_settings_model_obj10(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_pump_stop_empty_delay(value);
        }
    }
}

static void event_handler_cb_settings_model_obj11(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_messurement_delay(value);
        }
    }
}

static void event_handler_cb_settings_model_obj12(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_max_refuel_ml(value);
        }
    }
}

static void event_handler_cb_settings_model_obj13(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_max_defuel_ml(value);
        }
    }
}

static void event_handler_cb_settings_model_obj14(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_max_pressure(value);
        }
    }
}

static void event_handler_cb_settings_model_obj15(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_pump_stop_pressure_diff(value);
        }
    }
}

static void event_handler_cb_settings_model_obj16(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_pump_stop_pressure_empty(value);
        }
    }
}

static void event_handler_cb_settings_model_obj17(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_hopper_pressure(value);
        }
    }
}

static void event_handler_cb_settings_model_obj18(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_pump_stop_hopper_pressure_diff(value);
        }
    }
}

static void event_handler_cb_settings_model_obj19(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_fueling_last(value);
        }
    }
}

static void event_handler_cb_settings_model_obj20(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_fueling_count(value);
        }
    }
}

static void event_handler_cb_settings_model_obj21(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            const char *value = lv_textarea_get_text(ta);
            set_var_s_fueling_total(value);
        }
    }
}

static void event_handler_cb_pump_obj22(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_arc_get_value(ta);
            set_var_i_pump_pwr(value);
        }
    }
}

static void event_handler_cb_pump_bar_ml(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
}

static void event_handler_cb_header_btn_home(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
}

static void event_handler_cb_header_btn_settings(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
}

void create_screen_settings_system() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings_system = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_add_event_cb(obj, action_unload_system_settings, LV_EVENT_SCREEN_UNLOAD_START, (void *)0);
    add_style_page_style(obj);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj23 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 800, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_header(obj, 38);
        }
        {
            // contSysSettings
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_sys_settings = obj;
            lv_obj_set_pos(obj, 15, 65);
            lv_obj_set_size(obj, 770, 400);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            add_style_container_settings(obj);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 13);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Besitzer");
                    add_style_label_style(obj);
                }
                {
                    // txtOwner
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                    objects.txt_owner = obj;
                    lv_obj_set_pos(obj, 445, 0);
                    lv_obj_set_size(obj, 324, 47);
                    lv_textarea_set_max_length(obj, 40);
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                    lv_obj_add_event_cb(obj, action_keyboard_show, LV_EVENT_DEFOCUSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_keyboard_show, LV_EVENT_FOCUSED, (void *)1);
                    lv_obj_add_event_cb(obj, event_handler_cb_settings_system_txt_owner, LV_EVENT_ALL, 0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    add_style_textbox(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 120, 63);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "letztes Modell laden");
                    add_style_label_style(obj);
                }
                {
                    lv_obj_t *obj = lv_switch_create(parent_obj);
                    objects.obj0 = obj;
                    lv_obj_set_pos(obj, 0, 59);
                    lv_obj_set_size(obj, 70, 30);
                    lv_obj_add_event_cb(obj, event_handler_cb_settings_system_obj0, LV_EVENT_ALL, 0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff999999), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 120, 113);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Signalton");
                    add_style_label_style(obj);
                }
                {
                    lv_obj_t *obj = lv_switch_create(parent_obj);
                    objects.obj1 = obj;
                    lv_obj_set_pos(obj, 0, 109);
                    lv_obj_set_size(obj, 70, 30);
                    lv_obj_add_event_cb(obj, event_handler_cb_settings_system_obj1, LV_EVENT_ALL, 0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff999999), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 163);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Abschalterinnerung [s]");
                    add_style_label_style(obj);
                }
                {
                    // txtSysPowerOffTime
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                    objects.txt_sys_power_off_time = obj;
                    lv_obj_set_pos(obj, 445, 150);
                    lv_obj_set_size(obj, 324, 47);
                    lv_textarea_set_accepted_chars(obj, "0123456789");
                    lv_textarea_set_max_length(obj, 4);
                    lv_textarea_set_placeholder_text(obj, "off");
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                    lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                    lv_obj_add_event_cb(obj, event_handler_cb_settings_system_txt_sys_power_off_time, LV_EVENT_ALL, 0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    add_style_textbox(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 238);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Durchflussmesser [Impulse/Liter]");
                    add_style_label_style(obj);
                }
                {
                    // txtFlowTicks
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                    objects.txt_flow_ticks = obj;
                    lv_obj_set_pos(obj, 445, 225);
                    lv_obj_set_size(obj, 324, 47);
                    lv_textarea_set_accepted_chars(obj, "0123456789");
                    lv_textarea_set_max_length(obj, 5);
                    lv_textarea_set_placeholder_text(obj, "Ticks");
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                    lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                    lv_obj_add_event_cb(obj, event_handler_cb_settings_system_txt_flow_ticks, LV_EVENT_ALL, 0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    add_style_textbox(obj);
                }
                {
                    lv_obj_t *obj = lv_imagebutton_create(parent_obj);
                    lv_obj_set_pos(obj, 386, 229);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_imagebutton_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &img_settings, NULL);
                    lv_obj_add_event_cb(obj, action_go2settings_calibrate, LV_EVENT_CLICKED, (void *)0);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 288);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Drucksensor [Messwete/Mittelwert]");
                    add_style_label_style(obj);
                }
                {
                    // txtPressureAvg
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                    objects.txt_pressure_avg = obj;
                    lv_obj_set_pos(obj, 445, 275);
                    lv_obj_set_size(obj, 324, 47);
                    lv_textarea_set_accepted_chars(obj, "0123456789");
                    lv_textarea_set_max_length(obj, 2);
                    lv_textarea_set_placeholder_text(obj, "Anzahl");
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                    lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                    lv_obj_add_event_cb(obj, event_handler_cb_settings_system_txt_pressure_avg, LV_EVENT_ALL, 0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    add_style_textbox(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 338);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Minimalspannung Akku [V]");
                    add_style_label_style(obj);
                }
                {
                    // txtAkkuMinV
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                    objects.txt_akku_min_v = obj;
                    lv_obj_set_pos(obj, 445, 325);
                    lv_obj_set_size(obj, 323, 47);
                    lv_textarea_set_accepted_chars(obj, ".0123456789");
                    lv_textarea_set_max_length(obj, 4);
                    lv_textarea_set_placeholder_text(obj, "Volt");
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                    lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                    lv_obj_add_event_cb(obj, event_handler_cb_settings_system_txt_akku_min_v, LV_EVENT_ALL, 0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    add_style_textbox(obj);
                }
                {
                    lv_obj_t *obj = lv_imagebutton_create(parent_obj);
                    lv_obj_set_pos(obj, 386, 329);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, 40);
                    lv_imagebutton_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &img_settings, NULL);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 413);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Pumpenleistung");
                    add_style_label_style(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 463);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Kalibrierbetrieb [%]");
                    add_style_label_style(obj);
                }
                {
                    // txtPumpPwrCalib
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                    objects.txt_pump_pwr_calib = obj;
                    lv_obj_set_pos(obj, 445, 450);
                    lv_obj_set_size(obj, 324, 47);
                    lv_textarea_set_accepted_chars(obj, "0123456789");
                    lv_textarea_set_max_length(obj, 3);
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                    lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                    lv_obj_add_event_cb(obj, event_handler_cb_settings_system_txt_pump_pwr_calib, LV_EVENT_ALL, 0);
                    add_style_textbox(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 513);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "manueller Betrieb [%]");
                    add_style_label_style(obj);
                }
                {
                    // txtPumpPwrManu
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                    objects.txt_pump_pwr_manu = obj;
                    lv_obj_set_pos(obj, 446, 500);
                    lv_obj_set_size(obj, 324, 47);
                    lv_textarea_set_accepted_chars(obj, "0123456789");
                    lv_textarea_set_max_length(obj, 3);
                    lv_textarea_set_placeholder_text(obj, "---");
                    lv_textarea_set_one_line(obj, true);
                    lv_textarea_set_password_mode(obj, false);
                    lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                    lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                    lv_obj_add_event_cb(obj, event_handler_cb_settings_system_txt_pump_pwr_manu, LV_EVENT_ALL, 0);
                    add_style_textbox(obj);
                }
            }
        }
        {
            // contKeyboardSettings
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_keyboard_settings = obj;
            lv_obj_set_pos(obj, 1, 162);
            lv_obj_set_size(obj, 798, 318);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_keyboard(obj, 45);
        }
        {
            // contNumpadSettings
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_numpad_settings = obj;
            lv_obj_set_pos(obj, 27, 70);
            lv_obj_set_size(obj, 355, 395);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_numpad(obj, 47);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj24 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_save(obj, 49);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 415, 216);
                    lv_obj_set_size(obj, 230, 90);
                    lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_CLICKED, (void *)9);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Nein");
                            add_style_label_model_style(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 155, 216);
                    lv_obj_set_size(obj, 230, 90);
                    lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_CLICKED, (void *)8);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Ja");
                            add_style_label_model_style(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_settings_system();
}

void tick_screen_settings_system() {
    tick_user_widget_header(38);
    {
        const char *new_val = get_var_s_owner();
        const char *cur_val = lv_textarea_get_text(objects.txt_owner);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.txt_owner;
            lv_textarea_set_text(objects.txt_owner, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_load_last_model();
        bool cur_val = lv_obj_has_state(objects.obj0, LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj0;
            if (new_val) lv_obj_add_state(objects.obj0, LV_STATE_CHECKED);
            else lv_obj_clear_state(objects.obj0, LV_STATE_CHECKED);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_signal();
        bool cur_val = lv_obj_has_state(objects.obj1, LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj1;
            if (new_val) lv_obj_add_state(objects.obj1, LV_STATE_CHECKED);
            else lv_obj_clear_state(objects.obj1, LV_STATE_CHECKED);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_sys_power_off_time();
        const char *cur_val = lv_textarea_get_text(objects.txt_sys_power_off_time);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.txt_sys_power_off_time;
            lv_textarea_set_text(objects.txt_sys_power_off_time, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_flow_ticks();
        const char *cur_val = lv_textarea_get_text(objects.txt_flow_ticks);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.txt_flow_ticks;
            lv_textarea_set_text(objects.txt_flow_ticks, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_pressure_avg();
        const char *cur_val = lv_textarea_get_text(objects.txt_pressure_avg);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.txt_pressure_avg;
            lv_textarea_set_text(objects.txt_pressure_avg, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_akku_min_v();
        const char *cur_val = lv_textarea_get_text(objects.txt_akku_min_v);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.txt_akku_min_v;
            lv_textarea_set_text(objects.txt_akku_min_v, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_pump_pwr_calib();
        const char *cur_val = lv_textarea_get_text(objects.txt_pump_pwr_calib);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.txt_pump_pwr_calib;
            lv_textarea_set_text(objects.txt_pump_pwr_calib, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_pump_pwr_manu();
        const char *cur_val = lv_textarea_get_text(objects.txt_pump_pwr_manu);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.txt_pump_pwr_manu;
            lv_textarea_set_text(objects.txt_pump_pwr_manu, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_keyboard();
        bool cur_val = lv_obj_has_flag(objects.cont_keyboard_settings, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.cont_keyboard_settings;
            if (new_val) lv_obj_add_flag(objects.cont_keyboard_settings, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.cont_keyboard_settings, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    tick_user_widget_keyboard(45);
    {
        bool new_val = get_var_b_hide_numpad();
        bool cur_val = lv_obj_has_flag(objects.cont_numpad_settings, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.cont_numpad_settings;
            if (new_val) lv_obj_add_flag(objects.cont_numpad_settings, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.cont_numpad_settings, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    tick_user_widget_numpad(47);
    {
        bool new_val = get_var_b_hide_dialog_save_system();
        bool cur_val = lv_obj_has_flag(objects.obj24, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj24;
            if (new_val) lv_obj_add_flag(objects.obj24, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj24, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    tick_user_widget_save(49);
}

void create_screen_settings() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    add_style_page_style(obj);
    lv_obj_set_style_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj25 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 800, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_header(obj, 51);
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            lv_obj_set_pos(obj, 57, 158);
            lv_obj_set_size(obj, 195, 182);
            lv_obj_add_event_cb(obj, action_go2settings_system, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "System");
                    add_style_label_model_style(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            lv_obj_set_pos(obj, 303, 159);
            lv_obj_set_size(obj, 195, 182);
            lv_obj_add_event_cb(obj, action_go2settings_calibrate, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_state(obj, LV_STATE_DISABLED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Kalibrieren");
                    add_style_label_model_style(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            lv_obj_set_pos(obj, 549, 158);
            lv_obj_set_size(obj, 195, 182);
            lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_CLICKED, (void *)11);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "hmm...");
                    add_style_label_model_style(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
    
    tick_screen_settings();
}

void tick_screen_settings() {
    tick_user_widget_header(51);
}

void create_screen_model_select() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.model_select = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    add_style_page_style(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj26 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 800, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_header(obj, 57);
        }
        {
            // contModels
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_models = obj;
            lv_obj_set_pos(obj, 16, 65);
            lv_obj_set_size(obj, 768, 401);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            add_style_container_settings(obj);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW_WRAP, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 23);
                    lv_obj_set_size(obj, 378, 109);
                    lv_obj_add_event_cb(obj, action_go2_manuel_pump, LV_EVENT_CLICKED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Manuell Tanken");
                            add_style_label_model_style(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 234);
                    lv_obj_set_size(obj, 378, 109);
                    lv_obj_add_event_cb(obj, action_go2model, LV_EVENT_CLICKED, (void *)55);
                    lv_obj_add_event_cb(obj, action_go2settings_models, LV_EVENT_LONG_PRESSED, (void *)5);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_PCT(106), LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "zur Entwicklung - static");
                            add_style_label_model_style(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 390, 234);
                    lv_obj_set_size(obj, 378, 109);
                    lv_obj_add_event_cb(obj, action_go2new_model, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_style_bg_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // lblModelPlus
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.lbl_model_plus = obj;
                            lv_obj_set_pos(obj, 0, 1);
                            lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "+ zur entwicklung");
                            add_style_label_model_style(obj);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xff5b5b5b), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj27 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 800, 480);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_wait(obj, 65);
        }
    }
    
    tick_screen_model_select();
}

void tick_screen_model_select() {
    tick_user_widget_header(57);
    {
        bool new_val = get_var_b_hide_wait();
        bool cur_val = lv_obj_has_flag(objects.obj27, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj27;
            if (new_val) lv_obj_add_flag(objects.obj27, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj27, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    tick_user_widget_wait(65);
}

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    add_style_page_style(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // logo
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.logo = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_logo);
            lv_obj_add_event_cb(obj, action_go2settings, LV_EVENT_PRESSED, (void *)0);
        }
        {
            lv_obj_t *obj = lv_imagebutton_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, 480);
            lv_imagebutton_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &img_logo, NULL);
            lv_obj_add_event_cb(obj, action_go2home, LV_EVENT_CLICKED, (void *)0);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj28 = obj;
            lv_obj_set_pos(obj, 12, 416);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "");
            add_style_label_style(obj);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfff6f8f7), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj29 = obj;
            lv_obj_set_pos(obj, 12, 442);
            lv_obj_set_size(obj, LV_PCT(97), LV_SIZE_CONTENT);
            lv_label_set_text(obj, "");
            add_style_label_style(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff161a23), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj33 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            add_style_container_settings(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_msgbox_create(parent_obj);
                    objects.obj30 = obj;
                    lv_obj_set_pos(obj, 136, 78);
                    lv_obj_set_size(obj, 528, 325);
                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff68625d), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj31 = obj;
                    lv_obj_set_pos(obj, -4, 109);
                    lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "ERROR");
                    add_style_label_style(obj);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffad1717), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_decor(obj, LV_TEXT_DECOR_UNDERLINE, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj32 = obj;
                    lv_obj_set_pos(obj, 142, 202);
                    lv_obj_set_size(obj, 510, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Es kann keine Verbindung zur Tankstation aufgebaut werden.");
                    add_style_label_style(obj);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff970909), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 145, 300);
                    lv_obj_set_size(obj, 510, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "- Anlage vom Akku trennen.\n- Steckverbindungen kontrollieren.\n- Akku wieder anstecken.");
                    add_style_label_style(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
    {
        const char *new_val = get_var_s_owner();
        const char *cur_val = lv_label_get_text(objects.obj28);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj28;
            lv_label_set_text(objects.obj28, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_status();
        const char *cur_val = lv_label_get_text(objects.obj29);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj29;
            lv_label_set_text(objects.obj29, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_box_start_error();
        bool cur_val = lv_obj_has_flag(objects.obj33, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj33;
            if (new_val) lv_obj_add_flag(objects.obj33, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj33, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_settings_model() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings_model = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_add_event_cb(obj, action_unload_model_settings, LV_EVENT_SCREEN_UNLOAD_START, (void *)0);
    add_style_page_style(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj34 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 800, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_header(obj, 74);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            // contModelSettings
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_model_settings = obj;
            lv_obj_set_pos(obj, 15, 65);
            lv_obj_set_size(obj, 770, 400);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            add_style_container_settings(obj);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_row(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 8);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Modell");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj2 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 40);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_keyboard_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_keyboard_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj2, LV_EVENT_ALL, 0);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLL_CHAIN_VER);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 58);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Tanktype");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.obj3 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, LV_SIZE_CONTENT);
                            lv_dropdown_set_options(obj, "Beutel\n1 Tank\n2 Tank");
                            lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_VALUE_CHANGED, (void *)13);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj3, LV_EVENT_ALL, 0);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj36 = obj;
                    lv_obj_set_pos(obj, 0, 108);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Menge");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj37 = obj;
                    lv_obj_set_pos(obj, 0, 158);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Pumpleistung [%]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj4 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_accepted_chars(obj, "0123456789");
                            lv_textarea_set_max_length(obj, 3);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj4, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj38 = obj;
                    lv_obj_set_pos(obj, 0, 208);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Druckabfall bei Schlauchbruch [mbar]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj5 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj5, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 258);
                    lv_obj_set_size(obj, 769, 20);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            static lv_point_precise_t line_points[] = {
                                { 50, 0 },
                                { 700, 0 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_pos(obj, 0, 9);
                            lv_obj_set_size(obj, LV_PCT(100), 10);
                            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj39 = obj;
                    lv_obj_set_pos(obj, 0, 968);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Wert 0 = Funktion deaktivieren");
                            add_style_label_style(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 258);
                    lv_obj_set_size(obj, 769, 20);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            static lv_point_precise_t line_points[] = {
                                { 50, 0 },
                                { 700, 0 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_pos(obj, 0, 9);
                            lv_obj_set_size(obj, LV_PCT(100), 10);
                            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj40 = obj;
                    lv_obj_set_pos(obj, 0, 278);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Maximale Betankzeit [s]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj6 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj6, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj41 = obj;
                    lv_obj_set_pos(obj, 1, 328);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Maximale Enttankzeit [s]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj7 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj7, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj42 = obj;
                    lv_obj_set_pos(obj, 0, 378);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Ruecktankzeit [s]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj8 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj8, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj43 = obj;
                    lv_obj_set_pos(obj, 0, 968);
                    lv_obj_set_size(obj, 769, 47);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 3);
                            lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Luftblasenentfernung beginnt wenn\nDruck > Leerlaufabschaltdruck");
                            add_style_label_style(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj44 = obj;
                    lv_obj_set_pos(obj, 0, 428);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Luftblasenentfernung [s]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj9 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_go2home, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj9, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj45 = obj;
                    lv_obj_set_pos(obj, 0, 478);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Abschaltzeit nach Leererkennung [s]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj10 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj10, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj46 = obj;
                    lv_obj_set_pos(obj, 0, 528);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Messverzoegerung [s]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj11 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_go2home, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj11, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 578);
                    lv_obj_set_size(obj, 769, 20);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            static lv_point_precise_t line_points[] = {
                                { 50, 0 },
                                { 700, 0 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_pos(obj, 0, 9);
                            lv_obj_set_size(obj, LV_PCT(100), 10);
                            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj47 = obj;
                    lv_obj_set_pos(obj, 0, 598);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Maximale Betankmenge [ml]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj12 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj12, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj48 = obj;
                    lv_obj_set_pos(obj, 0, 648);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Maximale Enttankmenge [ml]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj13 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj13, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj49 = obj;
                    lv_obj_set_pos(obj, 0, 698);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Maximaler Tankdruck [mbar]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj14 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj14, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj50 = obj;
                    lv_obj_set_pos(obj, 0, 748);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Differenzabschaltdruck [mbar]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj15 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj15, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj51 = obj;
                    lv_obj_set_pos(obj, 0, 968);
                    lv_obj_set_size(obj, 769, 70);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 3);
                            lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Beutel ist leer wenn\nDruck kleiner Leerlaufabschaltdruck \n&& DiffDruck == 0 && FlowRate <= 1 ml/s");
                            add_style_label_style(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj52 = obj;
                    lv_obj_set_pos(obj, 0, 798);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Leerlaufabschaltdruck [mbar]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj16 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj16, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj53 = obj;
                    lv_obj_set_pos(obj, 0, 848);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Maximaler Hopperfuelldruck [mbar]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj17 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj17, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj54 = obj;
                    lv_obj_set_pos(obj, 0, 898);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Differenzabschaltdruck Hopper [mbar]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj18 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_placeholder_text(obj, "---");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj18, LV_EVENT_ALL, 0);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 948);
                    lv_obj_set_size(obj, 769, 20);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            static lv_point_precise_t line_points[] = {
                                { 50, 0 },
                                { 700, 0 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_pos(obj, 0, 9);
                            lv_obj_set_size(obj, LV_PCT(100), 10);
                            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj55 = obj;
                    lv_obj_set_pos(obj, 0, 968);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Letzte Tankmenge [ml]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj19 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj19, LV_EVENT_ALL, 0);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj56 = obj;
                    lv_obj_set_pos(obj, 0, 1018);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Tankvorgaenge");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            lv_obj_set_pos(obj, 326, 4);
                            lv_obj_set_size(obj, 100, 40);
                            lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_LONG_PRESSED, (void *)3);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Reset");
                                    add_style_label_style(obj);
                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                            }
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj20 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj20, LV_EVENT_ALL, 0);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.obj57 = obj;
                    lv_obj_set_pos(obj, 0, 1068);
                    lv_obj_set_size(obj, 769, 50);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 13);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Gesamtmenge [L]");
                            add_style_label_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            lv_obj_set_pos(obj, 325, 4);
                            lv_obj_set_size(obj, 100, 40);
                            lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_LONG_PRESSED, (void *)4);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Reset");
                                    add_style_label_style(obj);
                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                            }
                        }
                        {
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.obj21 = obj;
                            lv_obj_set_pos(obj, 445, 0);
                            lv_obj_set_size(obj, 324, 47);
                            lv_textarea_set_max_length(obj, 128);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, event_handler_cb_settings_model_obj21, LV_EVENT_ALL, 0);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            add_style_textbox(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 1118);
                    lv_obj_set_size(obj, 769, 20);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            static lv_point_precise_t line_points[] = {
                                { 50, 0 },
                                { 700, 0 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_pos(obj, 0, 9);
                            lv_obj_set_size(obj, LV_PCT(100), 10);
                            lv_obj_set_style_line_width(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 1138);
                    lv_obj_set_size(obj, 769, 90);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    add_style_container_settings(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 24);
                            lv_obj_set_size(obj, 236, 50);
                            lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_LONG_PRESSED, (void *)5);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_text(obj, "Modell loeschen");
                                    add_style_label_style(obj);
                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                }
                            }
                        }
                    }
                }
            }
        }
        {
            // contKeyboardModel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_keyboard_model = obj;
            lv_obj_set_pos(obj, 1, 165);
            lv_obj_set_size(obj, 798, 318);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_keyboard(obj, 81);
        }
        {
            // contNumpadModel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_numpad_model = obj;
            lv_obj_set_pos(obj, 35, 70);
            lv_obj_set_size(obj, 355, 395);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_numpad(obj, 83);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj35 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 800, 480);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_save(obj, 85);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 415, 218);
                    lv_obj_set_size(obj, 230, 90);
                    lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_CLICKED, (void *)7);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Nein");
                            add_style_label_model_style(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 155, 218);
                    lv_obj_set_size(obj, 230, 90);
                    lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_CLICKED, (void *)6);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Ja");
                            add_style_label_model_style(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_settings_model();
}

void tick_screen_settings_model() {
    tick_user_widget_header(74);
    {
        const char *new_val = get_var_s_modelname();
        const char *cur_val = lv_textarea_get_text(objects.obj2);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj2;
            lv_textarea_set_text(objects.obj2, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        if (!(lv_obj_get_state(objects.obj3) & LV_STATE_EDITED)) {
            int32_t new_val = get_var_i_tank_type_model();
            int32_t cur_val = lv_dropdown_get_selected(objects.obj3);
            if (new_val != cur_val) {
                tick_value_change_obj = objects.obj3;
                lv_dropdown_set_selected(objects.obj3, new_val);
                tick_value_change_obj = NULL;
            }
        }
    }
    {
        bool new_val = get_var_b_hide_mod_menge();
        bool cur_val = lv_obj_has_flag(objects.obj36, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj36;
            if (new_val) lv_obj_add_flag(objects.obj36, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj36, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_pump_pwr();
        bool cur_val = lv_obj_has_flag(objects.obj37, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj37;
            if (new_val) lv_obj_add_flag(objects.obj37, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj37, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_pump_pwr_model();
        const char *cur_val = lv_textarea_get_text(objects.obj4);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj4;
            lv_textarea_set_text(objects.obj4, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_drop_hose_break();
        bool cur_val = lv_obj_has_flag(objects.obj38, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj38;
            if (new_val) lv_obj_add_flag(objects.obj38, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj38, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_pressure_drop_hose_break();
        const char *cur_val = lv_textarea_get_text(objects.obj5);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj5;
            lv_textarea_set_text(objects.obj5, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_pump_stop_pressure_empty();
        bool cur_val = lv_obj_has_flag(objects.obj39, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj39;
            if (new_val) lv_obj_add_flag(objects.obj39, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj39, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_max_refuel_time();
        bool cur_val = lv_obj_has_flag(objects.obj40, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj40;
            if (new_val) lv_obj_add_flag(objects.obj40, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj40, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_max_refuel_time();
        const char *cur_val = lv_textarea_get_text(objects.obj6);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj6;
            lv_textarea_set_text(objects.obj6, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_max_defuel_time();
        bool cur_val = lv_obj_has_flag(objects.obj41, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj41;
            if (new_val) lv_obj_add_flag(objects.obj41, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj41, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_max_defuel_time();
        const char *cur_val = lv_textarea_get_text(objects.obj7);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj7;
            lv_textarea_set_text(objects.obj7, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_back_fuel_time();
        bool cur_val = lv_obj_has_flag(objects.obj42, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj42;
            if (new_val) lv_obj_add_flag(objects.obj42, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj42, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_back_fuel_time();
        const char *cur_val = lv_textarea_get_text(objects.obj8);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj8;
            lv_textarea_set_text(objects.obj8, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_bag_info();
        bool cur_val = lv_obj_has_flag(objects.obj43, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj43;
            if (new_val) lv_obj_add_flag(objects.obj43, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj43, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_air_removal_time();
        bool cur_val = lv_obj_has_flag(objects.obj44, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj44;
            if (new_val) lv_obj_add_flag(objects.obj44, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj44, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_air_removal_time();
        const char *cur_val = lv_textarea_get_text(objects.obj9);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj9;
            lv_textarea_set_text(objects.obj9, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_pump_stop_empty_delay();
        bool cur_val = lv_obj_has_flag(objects.obj45, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj45;
            if (new_val) lv_obj_add_flag(objects.obj45, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj45, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_pump_stop_empty_delay();
        const char *cur_val = lv_textarea_get_text(objects.obj10);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj10;
            lv_textarea_set_text(objects.obj10, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_messurement_delay();
        bool cur_val = lv_obj_has_flag(objects.obj46, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj46;
            if (new_val) lv_obj_add_flag(objects.obj46, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj46, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_messurement_delay();
        const char *cur_val = lv_textarea_get_text(objects.obj11);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj11;
            lv_textarea_set_text(objects.obj11, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_max_refuel_ml();
        bool cur_val = lv_obj_has_flag(objects.obj47, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj47;
            if (new_val) lv_obj_add_flag(objects.obj47, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj47, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_max_refuel_ml();
        const char *cur_val = lv_textarea_get_text(objects.obj12);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj12;
            lv_textarea_set_text(objects.obj12, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_max_defuel_ml();
        bool cur_val = lv_obj_has_flag(objects.obj48, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj48;
            if (new_val) lv_obj_add_flag(objects.obj48, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj48, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_max_defuel_ml();
        const char *cur_val = lv_textarea_get_text(objects.obj13);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj13;
            lv_textarea_set_text(objects.obj13, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_max_pressure();
        bool cur_val = lv_obj_has_flag(objects.obj49, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj49;
            if (new_val) lv_obj_add_flag(objects.obj49, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj49, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_max_pressure();
        const char *cur_val = lv_textarea_get_text(objects.obj14);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj14;
            lv_textarea_set_text(objects.obj14, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_pump_stop_pressure_diff();
        bool cur_val = lv_obj_has_flag(objects.obj50, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj50;
            if (new_val) lv_obj_add_flag(objects.obj50, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj50, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_pump_stop_pressure_diff();
        const char *cur_val = lv_textarea_get_text(objects.obj15);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj15;
            lv_textarea_set_text(objects.obj15, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_bag_info();
        bool cur_val = lv_obj_has_flag(objects.obj51, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj51;
            if (new_val) lv_obj_add_flag(objects.obj51, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj51, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_pump_stop_pressure_empty();
        bool cur_val = lv_obj_has_flag(objects.obj52, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj52;
            if (new_val) lv_obj_add_flag(objects.obj52, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj52, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_pump_stop_pressure_empty();
        const char *cur_val = lv_textarea_get_text(objects.obj16);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj16;
            lv_textarea_set_text(objects.obj16, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_hopper_pressure();
        bool cur_val = lv_obj_has_flag(objects.obj53, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj53;
            if (new_val) lv_obj_add_flag(objects.obj53, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj53, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_hopper_pressure();
        const char *cur_val = lv_textarea_get_text(objects.obj17);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj17;
            lv_textarea_set_text(objects.obj17, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_pump_stop_hopper_pressure_diff();
        bool cur_val = lv_obj_has_flag(objects.obj54, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj54;
            if (new_val) lv_obj_add_flag(objects.obj54, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj54, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_pump_stop_hopper_pressure_diff();
        const char *cur_val = lv_textarea_get_text(objects.obj18);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj18;
            lv_textarea_set_text(objects.obj18, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_fueling_last();
        bool cur_val = lv_obj_has_flag(objects.obj55, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj55;
            if (new_val) lv_obj_add_flag(objects.obj55, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj55, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_fueling_last();
        const char *cur_val = lv_textarea_get_text(objects.obj19);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj19;
            lv_textarea_set_text(objects.obj19, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_fueling_count();
        bool cur_val = lv_obj_has_flag(objects.obj56, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj56;
            if (new_val) lv_obj_add_flag(objects.obj56, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj56, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_fueling_count();
        const char *cur_val = lv_textarea_get_text(objects.obj20);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj20;
            lv_textarea_set_text(objects.obj20, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_mod_fueling_total();
        bool cur_val = lv_obj_has_flag(objects.obj57, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj57;
            if (new_val) lv_obj_add_flag(objects.obj57, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj57, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_fueling_total();
        const char *cur_val = lv_textarea_get_text(objects.obj21);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj21;
            lv_textarea_set_text(objects.obj21, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_keyboard();
        bool cur_val = lv_obj_has_flag(objects.cont_keyboard_model, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.cont_keyboard_model;
            if (new_val) lv_obj_add_flag(objects.cont_keyboard_model, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.cont_keyboard_model, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    tick_user_widget_keyboard(81);
    {
        bool new_val = get_var_b_hide_numpad();
        bool cur_val = lv_obj_has_flag(objects.cont_numpad_model, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.cont_numpad_model;
            if (new_val) lv_obj_add_flag(objects.cont_numpad_model, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.cont_numpad_model, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    tick_user_widget_numpad(83);
    {
        bool new_val = get_var_b_hide_dialog_save_model();
        bool cur_val = lv_obj_has_flag(objects.obj35, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj35;
            if (new_val) lv_obj_add_flag(objects.obj35, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj35, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    tick_user_widget_save(85);
}

void create_screen_pump() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.pump = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_add_event_cb(obj, action_unload_manual_fuel, LV_EVENT_SCREEN_UNLOADED, (void *)0);
    lv_obj_add_state(obj, LV_STATE_DISABLED);
    add_style_page_style(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj58 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 800, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            create_user_widget_header(obj, 109);
        }
        {
            // contPumpMain
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_pump_main = obj;
            lv_obj_set_pos(obj, 15, 65);
            lv_obj_set_size(obj, 376, 402);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            add_style_container_settings(obj);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_arc_create(parent_obj);
                    objects.obj22 = obj;
                    lv_obj_set_pos(obj, 112, 12);
                    lv_obj_set_size(obj, 150, 150);
                    lv_arc_set_range(obj, 40, 100);
                    lv_arc_set_bg_start_angle(obj, 120);
                    lv_arc_set_bg_end_angle(obj, 60);
                    lv_obj_add_event_cb(obj, action_change_pump_pwr, LV_EVENT_VALUE_CHANGED, (void *)0);
                    lv_obj_add_event_cb(obj, event_handler_cb_pump_obj22, LV_EVENT_ALL, 0);
                    lv_obj_set_style_blend_mode(obj, LV_BLEND_MODE_MULTIPLY, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_width(obj, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_opa(obj, 150, LV_PART_MAIN | LV_STATE_DISABLED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff585e63), LV_PART_KNOB | LV_STATE_DISABLED);
                    lv_obj_set_style_bg_opa(obj, 200, LV_PART_KNOB | LV_STATE_DISABLED);
                    lv_obj_set_style_arc_color(obj, lv_color_hex(0xff585e63), LV_PART_INDICATOR | LV_STATE_DISABLED);
                    lv_obj_set_style_arc_opa(obj, 200, LV_PART_INDICATOR | LV_STATE_DISABLED);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj62 = obj;
                            lv_obj_set_pos(obj, 0, 9);
                            lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "");
                            add_style_label_model_style(obj);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 2, -14);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Power");
                            add_style_label_model_style(obj);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 46, 195);
                    lv_obj_set_size(obj, 122, 50);
                    lv_obj_add_event_cb(obj, action_pump_out, LV_EVENT_CLICKED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "OUT");
                            add_style_label_model_style(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 206, 195);
                    lv_obj_set_size(obj, 122, 50);
                    lv_obj_add_event_cb(obj, action_pump_in, LV_EVENT_CLICKED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "IN");
                            add_style_label_model_style(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.obj59 = obj;
                    lv_obj_set_pos(obj, 126, 267);
                    lv_obj_set_size(obj, 122, 122);
                    lv_obj_add_event_cb(obj, action_pump_stop, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "STOP");
                            add_style_label_model_style(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
        {
            // contFlowCalibrate
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_flow_calibrate = obj;
            lv_obj_set_pos(obj, 412, 65);
            lv_obj_set_size(obj, 376, 402);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            add_style_container_settings(obj);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, -166);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Impulse/Liter");
                    add_style_label_model_style(obj);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj60 = obj;
                    lv_obj_set_pos(obj, 0, -107);
                    lv_obj_set_size(obj, LV_PCT(50), LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    add_style_label_model_style(obj);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 150, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 24, 149);
                    lv_obj_set_size(obj, 150, 64);
                    lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_CLICKED, (void *)1);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Reset");
                            add_style_label_model_style(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 201, 149);
                    lv_obj_set_size(obj, 150, 64);
                    lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_CLICKED, (void *)2);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Set Flow");
                            add_style_label_model_style(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj61 = obj;
                    lv_obj_set_pos(obj, 4, 244);
                    lv_obj_set_size(obj, LV_PCT(98), LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "- Reset\n- genau 1000ml in Messbecher\n- mit \"Set Flow\" uebernehmen\n- oder nach Reset erneut messen");
                    add_style_label_style(obj);
                    lv_obj_set_style_pad_top(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // contModelCalibrate
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_model_calibrate = obj;
            lv_obj_set_pos(obj, 412, 65);
            lv_obj_set_size(obj, 376, 402);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            add_style_container_settings(obj);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 88, 60);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Menge ml");
                    add_style_label_style(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 296, 282);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "mBar");
                    add_style_label_style(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 12, 282);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Mittlerer Tankdruk");
                    add_style_label_style(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 12, 245);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Tankzeit");
                    add_style_label_style(obj);
                }
            }
        }
        {
            // contModelFuel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_model_fuel = obj;
            lv_obj_set_pos(obj, 412, 65);
            lv_obj_set_size(obj, 376, 402);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            add_style_container_settings(obj);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_imagebutton_create(parent_obj);
                    objects.obj63 = obj;
                    lv_obj_set_pos(obj, 330, 5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, 40);
                    lv_imagebutton_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &img_oil, NULL);
                    lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_CLICKED, (void *)10);
                    lv_obj_set_style_image_opa(obj, 150, LV_PART_MAIN | LV_STATE_PRESSED);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj64 = obj;
                    lv_obj_set_pos(obj, 0, -159);
                    lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    add_style_label_model_style(obj);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, -93);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Tankmenge ml");
                    add_style_label_model_style(obj);
                }
                {
                    // barMl
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.bar_ml = obj;
                    lv_obj_set_pos(obj, 19, 150);
                    lv_obj_set_size(obj, LV_PCT(90), 24);
                    lv_bar_set_range(obj, 0, 4512);
                    lv_obj_add_event_cb(obj, event_handler_cb_pump_bar_ml, LV_EVENT_ALL, 0);
                }
                {
                    // scaleML
                    lv_obj_t *obj = lv_scale_create(parent_obj);
                    objects.scale_ml = obj;
                    lv_obj_set_pos(obj, 19, 150);
                    lv_obj_set_size(obj, LV_PCT(90), 40);
                    lv_scale_set_mode(obj, LV_SCALE_MODE_HORIZONTAL_BOTTOM);
                    lv_scale_set_range(obj, 0, 4512);
                    lv_scale_set_total_tick_count(obj, 31);
                    lv_scale_set_major_tick_every(obj, 5);
                    lv_scale_set_label_show(obj, true);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 19, 217);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Status");
                    add_style_label_style(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj65 = obj;
                    lv_obj_set_pos(obj, 101, 217);
                    lv_obj_set_size(obj, 254, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    add_style_label_style(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 19, 305);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Abschaltmenge");
                    add_style_label_style(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj66 = obj;
                    lv_obj_set_pos(obj, 198, 305);
                    lv_obj_set_size(obj, 157, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    add_style_label_style(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 19, 336);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Abschaltzeit");
                    add_style_label_style(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 198, 336);
                    lv_obj_set_size(obj, 157, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "--- s");
                    add_style_label_style(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 19, 365);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Tankart");
                    add_style_label_style(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj67 = obj;
                    lv_obj_set_pos(obj, 198, 365);
                    lv_obj_set_size(obj, 157, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    add_style_label_style(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 19, 245);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Druck");
                    add_style_label_style(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj68 = obj;
                    lv_obj_set_pos(obj, 198, 245);
                    lv_obj_set_size(obj, 157, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    add_style_label_style(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 19, 273);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Diff. Druck");
                    add_style_label_style(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj69 = obj;
                    lv_obj_set_pos(obj, 198, 273);
                    lv_obj_set_size(obj, 157, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    add_style_label_style(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // contManuelFuel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_manuel_fuel = obj;
            lv_obj_set_pos(obj, 412, 65);
            lv_obj_set_size(obj, 376, 402);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            add_style_container_settings(obj);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_imagebutton_create(parent_obj);
                    objects.obj70 = obj;
                    lv_obj_set_pos(obj, 330, 5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, 40);
                    lv_imagebutton_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &img_oil, NULL);
                    lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_CLICKED, (void *)10);
                    lv_obj_set_style_image_opa(obj, 150, LV_PART_MAIN | LV_STATE_PRESSED);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj71 = obj;
                    lv_obj_set_pos(obj, 0, -93);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    add_style_label_model_style(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 19, 217);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Status");
                    add_style_label_style(obj);
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj72 = obj;
                    lv_obj_set_pos(obj, 198, 217);
                    lv_obj_set_size(obj, 157, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "");
                    add_style_label_style(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
    }
    
    tick_screen_pump();
}

void tick_screen_pump() {
    tick_user_widget_header(109);
    {
        bool new_val = get_var_b_pump_pwr_disabled();
        bool cur_val = lv_obj_has_state(objects.obj22, LV_STATE_DISABLED);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj22;
            if (new_val) lv_obj_add_state(objects.obj22, LV_STATE_DISABLED);
            else lv_obj_clear_state(objects.obj22, LV_STATE_DISABLED);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_i_pump_pwr();
        int32_t cur_val = lv_arc_get_value(objects.obj22);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj22;
            lv_arc_set_value(objects.obj22, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_pump_pwr();
        const char *cur_val = lv_label_get_text(objects.obj62);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj62;
            lv_label_set_text(objects.obj62, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_cont_flow_calibrate();
        bool cur_val = lv_obj_has_flag(objects.cont_flow_calibrate, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.cont_flow_calibrate;
            if (new_val) lv_obj_add_flag(objects.cont_flow_calibrate, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.cont_flow_calibrate, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_i_calib_flow_sensor();
        const char *cur_val = lv_label_get_text(objects.obj60);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj60;
            lv_label_set_text(objects.obj60, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_cont_model_fuel();
        bool cur_val = lv_obj_has_flag(objects.cont_model_calibrate, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.cont_model_calibrate;
            if (new_val) lv_obj_add_flag(objects.cont_model_calibrate, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.cont_model_calibrate, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_model_fuel();
        bool cur_val = lv_obj_has_flag(objects.cont_model_fuel, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.cont_model_fuel;
            if (new_val) lv_obj_add_flag(objects.cont_model_fuel, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.cont_model_fuel, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_pump_save_mod_to_controller();
        bool cur_val = lv_obj_has_flag(objects.obj63, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj63;
            if (new_val) lv_obj_add_flag(objects.obj63, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj63, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_modelname();
        const char *cur_val = lv_label_get_text(objects.obj64);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj64;
            lv_label_set_text(objects.obj64, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_i_fuel_ml();
        int32_t cur_val = lv_bar_get_value(objects.bar_ml);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.bar_ml;
            lv_bar_set_value(objects.bar_ml, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_status();
        const char *cur_val = lv_label_get_text(objects.obj65);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj65;
            lv_label_set_text(objects.obj65, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_cut_off_ml();
        const char *cur_val = lv_label_get_text(objects.obj66);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj66;
            lv_label_set_text(objects.obj66, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_tank_type();
        const char *cur_val = lv_label_get_text(objects.obj67);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj67;
            lv_label_set_text(objects.obj67, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_fuel_mbar();
        const char *cur_val = lv_label_get_text(objects.obj68);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj68;
            lv_label_set_text(objects.obj68, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_fuel_diff_mbar();
        const char *cur_val = lv_label_get_text(objects.obj69);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj69;
            lv_label_set_text(objects.obj69, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_manuel_fuel();
        bool cur_val = lv_obj_has_flag(objects.cont_manuel_fuel, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.cont_manuel_fuel;
            if (new_val) lv_obj_add_flag(objects.cont_manuel_fuel, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.cont_manuel_fuel, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_hide_pump_save_mod_to_controller();
        bool cur_val = lv_obj_has_flag(objects.obj70, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj70;
            if (new_val) lv_obj_add_flag(objects.obj70, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.obj70, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_fuel_ml();
        const char *cur_val = lv_label_get_text(objects.obj71);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj71;
            lv_label_set_text(objects.obj71, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_status();
        const char *cur_val = lv_label_get_text(objects.obj72);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj72;
            lv_label_set_text(objects.obj72, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_user_widget_background(lv_obj_t *parent_obj, int startWidgetIndex) {
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_logo);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
    }
}

void tick_user_widget_background(int startWidgetIndex) {
}

void create_user_widget_header(lv_obj_t *parent_obj, int startWidgetIndex) {
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            add_style_container_settings(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // btnHome
            lv_obj_t *obj = lv_imagebutton_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 5, 6);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_imagebutton_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &img_home_airplane, NULL);
            lv_obj_add_event_cb(obj, action_go2home, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_event_cb(obj, event_handler_cb_header_btn_home, LV_EVENT_ALL, 0);
            lv_obj_set_style_image_opa(obj, 150, LV_PART_MAIN | LV_STATE_DISABLED);
            lv_obj_set_style_image_recolor(obj, lv_color_hex(0xff7b7b7b), LV_PART_MAIN | LV_STATE_DISABLED);
            lv_obj_set_style_image_recolor_opa(obj, 50, LV_PART_MAIN | LV_STATE_DISABLED);
            lv_obj_set_style_image_opa(obj, 150, LV_PART_MAIN | LV_STATE_PRESSED);
        }
        {
            // btnSettings
            lv_obj_t *obj = lv_imagebutton_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
            lv_obj_set_pos(obj, 755, 5);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_imagebutton_set_src(obj, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &img_settings, NULL);
            lv_imagebutton_set_src(obj, LV_IMAGEBUTTON_STATE_PRESSED, NULL, &img_settings, NULL);
            lv_imagebutton_set_src(obj, LV_IMAGEBUTTON_STATE_DISABLED, NULL, &img_settings, NULL);
            lv_obj_add_event_cb(obj, action_go2settings, LV_EVENT_RELEASED, (void *)0);
            lv_obj_add_event_cb(obj, event_handler_cb_header_btn_settings, LV_EVENT_ALL, 0);
            lv_obj_set_style_image_opa(obj, 150, LV_PART_MAIN | LV_STATE_DISABLED);
            lv_obj_set_style_image_recolor(obj, lv_color_hex(0xff7b7b7b), LV_PART_MAIN | LV_STATE_DISABLED);
            lv_obj_set_style_image_recolor_opa(obj, 50, LV_PART_MAIN | LV_STATE_DISABLED);
            lv_obj_set_style_image_opa(obj, 150, LV_PART_MAIN | LV_STATE_PRESSED);
        }
        {
            // lblScreenTitel
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 3] = obj;
            lv_obj_set_pos(obj, 149, 9);
            lv_obj_set_size(obj, 503, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "");
            lv_obj_add_event_cb(obj, action_event_button_click, LV_EVENT_LONG_PRESSED, (void *)11);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            add_style_label_style(obj);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // lblBatt
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 4] = obj;
            lv_obj_set_pos(obj, 603, 12);
            lv_obj_set_size(obj, 140, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "");
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            add_style_label_style(obj);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void tick_user_widget_header(int startWidgetIndex) {
    {
        bool new_val = get_var_b_disable_btn_home();
        bool cur_val = lv_obj_has_state(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_STATE_DISABLED);
        if (new_val != cur_val) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 0];
            if (new_val) lv_obj_add_state(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_STATE_DISABLED);
            else lv_obj_clear_state(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_STATE_DISABLED);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_b_disable_btn_settings();
        bool cur_val = lv_obj_has_state(((lv_obj_t **)&objects)[startWidgetIndex + 1], LV_STATE_DISABLED);
        if (new_val != cur_val) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 1];
            if (new_val) lv_obj_add_state(((lv_obj_t **)&objects)[startWidgetIndex + 1], LV_STATE_DISABLED);
            else lv_obj_clear_state(((lv_obj_t **)&objects)[startWidgetIndex + 1], LV_STATE_DISABLED);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_screen_titel();
        const char *cur_val = lv_label_get_text(((lv_obj_t **)&objects)[startWidgetIndex + 3]);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 3];
            lv_label_set_text(((lv_obj_t **)&objects)[startWidgetIndex + 3], new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_s_akku_volt();
        const char *cur_val = lv_label_get_text(((lv_obj_t **)&objects)[startWidgetIndex + 4]);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 4];
            lv_label_set_text(((lv_obj_t **)&objects)[startWidgetIndex + 4], new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_user_widget_keyboard(lv_obj_t *parent_obj, int startWidgetIndex) {
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            // kbKeyboard
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 798, 318);
            lv_obj_add_event_cb(obj, action_keyboard_show, LV_EVENT_READY, (void *)0);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void tick_user_widget_keyboard(int startWidgetIndex) {
    {
        bool new_val = get_var_b_hide_keyboard();
        bool cur_val = lv_obj_has_flag(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 0];
            if (new_val) lv_obj_add_flag(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
}

void create_user_widget_numpad(lv_obj_t *parent_obj, int startWidgetIndex) {
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            // kbNumpad
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 355, 395);
            lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_NUMBER);
            lv_obj_add_event_cb(obj, action_numpad_show, LV_EVENT_READY, (void *)0);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_30, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void tick_user_widget_numpad(int startWidgetIndex) {
    {
        bool new_val = get_var_b_hide_numpad();
        bool cur_val = lv_obj_has_flag(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 0];
            if (new_val) lv_obj_add_flag(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
}

void create_user_widget_save(lv_obj_t *parent_obj, int startWidgetIndex) {
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            // wndSaveSystemSettings
            lv_obj_t *obj = lv_win_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 125, 90);
            lv_obj_set_size(obj, 550, 300);
            lv_obj_set_style_border_width(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 277, 97);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Speichern");
            add_style_label_style(obj);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void tick_user_widget_save(int startWidgetIndex) {
}

void create_user_widget_wait(lv_obj_t *parent_obj, int startWidgetIndex) {
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            add_style_container_settings(obj);
            lv_obj_set_style_bg_opa(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_spinner_create(parent_obj);
            lv_spinner_set_anim_params(obj, 1000, 60);
            lv_obj_set_pos(obj, 360, 200);
            lv_obj_set_size(obj, 80, 80);
        }
    }
}

void tick_user_widget_wait(int startWidgetIndex) {
    {
        bool new_val = get_var_b_hide_wait();
        bool cur_val = lv_obj_has_flag(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 0];
            if (new_val) lv_obj_add_flag(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_settings_system,
    tick_screen_settings,
    tick_screen_model_select,
    tick_screen_main,
    tick_screen_settings_model,
    tick_screen_pump,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_settings_system();
    create_screen_settings();
    create_screen_model_select();
    create_screen_main();
    create_screen_settings_model();
    create_screen_pump();
}
