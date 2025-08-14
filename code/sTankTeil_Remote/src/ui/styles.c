#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: LabelStyle
//

void init_style_label_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &lv_font_montserrat_20);
    lv_style_set_text_color(style, lv_color_hex(0xff000000));
};

lv_style_t *get_style_label_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_label_style_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_label_style(lv_obj_t *obj) {
    lv_obj_add_style(obj, get_style_label_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_label_style(lv_obj_t *obj) {
    lv_obj_remove_style(obj, get_style_label_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: Container Settings
//

void init_style_container_settings_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xffffffff));
    lv_style_set_bg_opa(style, 150);
    lv_style_set_border_color(style, lv_color_hex(0xff252525));
    lv_style_set_border_opa(style, 222);
    lv_style_set_border_side(style, LV_BORDER_SIDE_NONE);
};

lv_style_t *get_style_container_settings_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_container_settings_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_container_settings(lv_obj_t *obj) {
    lv_obj_add_style(obj, get_style_container_settings_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_container_settings(lv_obj_t *obj) {
    lv_obj_remove_style(obj, get_style_container_settings_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: Textbox
//

void init_style_textbox_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &lv_font_montserrat_18);
};

lv_style_t *get_style_textbox_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_textbox_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_textbox(lv_obj_t *obj) {
    lv_obj_add_style(obj, get_style_textbox_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_textbox(lv_obj_t *obj) {
    lv_obj_remove_style(obj, get_style_textbox_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: Roller
//

void init_style_roller_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &lv_font_montserrat_18);
};

lv_style_t *get_style_roller_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_roller_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_roller(lv_obj_t *obj) {
    lv_obj_add_style(obj, get_style_roller_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_roller(lv_obj_t *obj) {
    lv_obj_remove_style(obj, get_style_roller_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: PageStyle
//

void init_style_page_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xff2c2a2a));
    lv_style_set_bg_grad_dir(style, LV_GRAD_DIR_VER);
    lv_style_set_bg_grad_color(style, lv_color_hex(0xffc1c1c1));
    lv_style_set_bg_grad_stop(style, 100);
    lv_style_set_bg_opa(style, 100);
};

lv_style_t *get_style_page_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_page_style_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_page_style(lv_obj_t *obj) {
    lv_obj_add_style(obj, get_style_page_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_page_style(lv_obj_t *obj) {
    lv_obj_remove_style(obj, get_style_page_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: LabelModelStyle
//

void init_style_label_model_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_align(style, LV_ALIGN_CENTER);
    lv_style_set_text_font(style, &lv_font_montserrat_24);
    lv_style_set_text_color(style, lv_color_hex(0xff212121));
    lv_style_set_text_align(style, LV_TEXT_ALIGN_CENTER);
};

lv_style_t *get_style_label_model_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_label_model_style_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_label_model_style(lv_obj_t *obj) {
    lv_obj_add_style(obj, get_style_label_model_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_label_model_style(lv_obj_t *obj) {
    lv_obj_remove_style(obj, get_style_label_model_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_label_style,
        add_style_container_settings,
        add_style_textbox,
        add_style_roller,
        add_style_page_style,
        add_style_label_model_style,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_label_style,
        remove_style_container_settings,
        remove_style_textbox,
        remove_style_roller,
        remove_style_page_style,
        remove_style_label_model_style,
    };
    remove_style_funcs[styleIndex](obj);
}

