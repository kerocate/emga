#pragma once
#include "lvgl/lvgl.h"

lv_obj_t *base_layout(void)
{
    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 800, 480);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

    static lv_style_t title_style;
    lv_style_init(&title_style);
    lv_style_set_bg_color(&title_style,lv_palette_main(LV_PALETTE_LIGHT_BLUE));

    lv_obj_t *obj;
    obj = lv_obj_create(cont);
    lv_obj_set_size(obj, 40, 480); /*Fix size*/
    lv_obj_add_style(obj,&title_style,0);


    obj = lv_obj_create(cont);
    lv_obj_set_height(obj, 40);
    lv_obj_set_flex_grow(obj, 1); /*1 portion from the free space*/

    obj = lv_obj_create(cont);
    lv_obj_set_height(obj, 40);
    lv_obj_set_flex_grow(obj, 2); /*2 portion from the free space*/

    obj = lv_obj_create(cont);
    lv_obj_set_size(obj, 40, 40); /*Fix size. It is flushed to the right by the "grow" items*/

    return cont;
}