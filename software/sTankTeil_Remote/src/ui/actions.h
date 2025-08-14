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

#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_go2home(lv_event_t * e);
extern void action_go2settings(lv_event_t * e);
extern void action_keyboard_show(lv_event_t * e);
extern void action_numpad_show(lv_event_t * e);
extern void action_go2settings_system(lv_event_t * e);
extern void action_go2settings_calibrate(lv_event_t * e);
extern void action_go2settings_models(lv_event_t * e);
extern void action_event_button_click(lv_event_t * e);
extern void action_pump_stop(lv_event_t * e);
extern void action_pump_in(lv_event_t * e);
extern void action_pump_out(lv_event_t * e);
extern void action_go2model(lv_event_t * e);
extern void action_go2_manuel_pump(lv_event_t * e);
extern void action_unload_manual_fuel(lv_event_t * e);
extern void action_unload_system_settings(lv_event_t * e);
extern void action_unload_model_settings(lv_event_t * e);
extern void action_change_pump_pwr(lv_event_t * e);
extern void action_go2new_model(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/