#pragma once

#include "lvgl/src/misc/lv_style.h"

// 样式对象
static lv_style_t default_style;
static lv_style_t title_style;

void init_base_style()
{
    // default
    lv_style_init(&default_style);
    lv_style_set_radius(&title_style, 0);
    lv_style_set_border_width(&title_style, 0);

    // title
    lv_style_init(&title_style);
    lv_style_set_bg_color(&title_style, lv_palette_main(LV_PALETTE_LIGHT_BLUE));
    lv_style_set_shadow_width(&title_style, 10);
    lv_style_set_shadow_ofs_x(&title_style, 5);
    lv_style_set_shadow_color(&title_style, lv_palette_main(LV_PALETTE_BLUE));

    
}