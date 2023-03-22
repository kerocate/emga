#pragma once
// #include "lvgl/lvgl.h"
#include "lvgl/src/core/lv_obj.h"
#include "lvgl/src/core/lv_obj_pos.h"
#include "lvgl/src/extra/layouts/flex/lv_flex.h"

#include "style/base_style.h"

lv_obj_t *base_layout(void)
{
    // bg
    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(cont);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(cont, 800, 480);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_add_style(cont, &default_style, 0);

    // title
    lv_obj_t *obj;
    obj = lv_obj_create(cont);
    lv_obj_remove_style_all(obj);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(obj, 100, 480); /*Fix size*/
    lv_obj_add_style(obj, &default_style, 0);
    lv_obj_add_style(obj, &title_style, 0);

    return cont;
}