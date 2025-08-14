#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: LabelStyle
lv_style_t *get_style_label_style_MAIN_DEFAULT();
void add_style_label_style(lv_obj_t *obj);
void remove_style_label_style(lv_obj_t *obj);

// Style: Container Settings
lv_style_t *get_style_container_settings_MAIN_DEFAULT();
void add_style_container_settings(lv_obj_t *obj);
void remove_style_container_settings(lv_obj_t *obj);

// Style: Textbox
lv_style_t *get_style_textbox_MAIN_DEFAULT();
void add_style_textbox(lv_obj_t *obj);
void remove_style_textbox(lv_obj_t *obj);

// Style: Roller
lv_style_t *get_style_roller_MAIN_DEFAULT();
void add_style_roller(lv_obj_t *obj);
void remove_style_roller(lv_obj_t *obj);

// Style: PageStyle
lv_style_t *get_style_page_style_MAIN_DEFAULT();
void add_style_page_style(lv_obj_t *obj);
void remove_style_page_style(lv_obj_t *obj);

// Style: LabelModelStyle
lv_style_t *get_style_label_model_style_MAIN_DEFAULT();
void add_style_label_model_style(lv_obj_t *obj);
void remove_style_label_model_style(lv_obj_t *obj);



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/