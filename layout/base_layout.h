#pragma once

#include "lvgl/lvgl.h"

#include "style/base_style.h"

static lv_obj_t *cont, *title, *content;

void base_layout(void)
{
    // bg
    cont = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(cont, 800, 480);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_REVERSE);
    lv_obj_add_style(cont, &default_style, 0);

    // content
    content = lv_obj_create(cont);
    // lv_obj_remove_style_all(content);
    lv_obj_set_size(content, 700, 480);
    lv_obj_add_style(content, &default_style, 0);
    lv_obj_add_style(content, &content_bg, 0);
    lv_obj_move_background(content);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_AUTO);

    // title
    title = lv_obj_create(cont);
    // lv_obj_remove_style_all(title);
    lv_obj_clear_flag(title, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(title, 100, 480);
    lv_obj_add_style(title, &default_style, 0);
    lv_obj_add_style(title, &title_style, 0);
}