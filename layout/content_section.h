#pragma once

#include "lvgl/lvgl.h"

#include "style/base_style.h"
#include "layout/base_layout.h"

static lv_obj_t *section;

static const char *btnm_map[] = {"1", "2", "3", "4", "5", "\n",
                                 "6", "7", "8", "9", "0", "\n",
                                 "Action1", "Action2", ""};

// lv_obj_t *create_section(lv_obj_t *parent)
// {
//     lv_obj_t *this_section = lv_obj_create(parent);
//     return this_section;
// }

void test_section()
{
    section = lv_obj_create(content);
    lv_obj_set_width(section, lv_pct(100));
    lv_obj_add_style(section, &default_style, 0);
    lv_obj_set_style_bg_color(section, lv_palette_main(LV_PALETTE_AMBER),0);

    lv_obj_t *btnm1 = lv_btnmatrix_create(section);
    lv_btnmatrix_set_map(btnm1, btnm_map);
    lv_btnmatrix_set_btn_width(btnm1, 10, 2); /*Make "Action1" twice as wide as "Action2"*/
    lv_btnmatrix_set_btn_ctrl(btnm1, 10, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_btn_ctrl(btnm1, 11, LV_BTNMATRIX_CTRL_CHECKED);
    lv_obj_align(btnm1, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *btnm2 = lv_btnmatrix_create(section);
    lv_btnmatrix_set_map(btnm2, btnm_map);
    lv_btnmatrix_set_btn_width(btnm2, 10, 2); /*Make "Action1" twice as wide as "Action2"*/
    lv_btnmatrix_set_btn_ctrl(btnm2, 10, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_btn_ctrl(btnm2, 11, LV_BTNMATRIX_CTRL_CHECKED);
    lv_obj_align(btnm2, LV_ALIGN_CENTER, 0, 0);

    // section = lv_obj_create(content);
    // lv_obj_remove_style_all(section);
    // lv_obj_set_width(section, lv_pct(100));
    // lv_obj_add_style(section, &default_style, 0);
    // lv_obj_set_style_bg_color(section, lv_palette_main(LV_PALETTE_AMBER),0);
}