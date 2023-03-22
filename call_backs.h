#pragma once
#include "main.h"
#include "lvgl/lvgl.h"

static int pressed;

static void *tick_thread(void *args)
{
    while (1)
    {
        usleep(5 * 1000); /*Sleep for 5 millisecond*/
        lv_tick_inc(5);   /*Tell LVGL that 5 milliseconds were elapsed*/
    }
}

static void flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *buf)
{
    int width = lv_area_get_width(area);
    int height = lv_area_get_height(area);
    uint32_t *row = &fbp[area->y1 * width + area->x1];
    // size_t row_size = width * vinfo.bits_per_pixel / 8;

    // Log the width, height, framebuffer width, and row size
    // printf("area->y: %d\n", area->y1);
    // printf("area->x: %d\n", area->x1);
    // printf("Width: %d\n", width);
    // printf("Height: %d\n", height);
    // printf("Framebuffer width: %d\n", fb_width);
    // printf("Row size: %zu\n\n", row_size);

    memcpy(row, buf, width * height * vinfo.bits_per_pixel / 8);
    // buf += width;

    // for (int y = 0; y < height; y++)
    // {
    //     memcpy(row, buf, width * vinfo.bits_per_pixel / 8);
    //     buf += width;
    //     row += width;
    // }
    fsync(lcdfd);
    lv_disp_flush_ready(disp_drv);
}

// type         code            data
// EV_KEY-1     BTN_TOUCH-330   1 down/ 0 up
// EV_ABS-3     ABS_Y-1         Yaxis
// EV_ABS-3     ABS_X-0         Xaxis
static void touch_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    // lv_task_handler(); /*Call `lv_task_handler()` manually to process the wake-up event*/
    // data->continue_reading = true;
    ssize_t n = read(tc_fd, &tc_ev, sizeof(tc_ev));
    // ssize_t n = read(fd, ev, sizeof(struct input_event) * MAX_EVENTS);

    // printf("type: %d ", tc_ev.type);
    // printf("code: %d ", tc_ev.code);
    // printf("value: %d\n\n", tc_ev.value);
    if (n < sizeof(tc_ev))
    {
        return;
    }
    if (tc_ev.type == EV_KEY)
    {
        if (tc_ev.code == BTN_TOUCH)
        {
            if (tc_ev.value == 1)
            {
                pressed = 1;
                data->state = LV_INDEV_STATE_PR;
                lv_event_send(lv_indev_get_obj_act(), LV_EVENT_PRESSED, NULL);
            }
            else
            {
                pressed = 0;
                data->state = LV_INDEV_STATE_REL;
                lv_event_send(lv_indev_get_obj_act(), LV_EVENT_RELEASED, NULL);
            }
        }
    }
    else if (tc_ev.type == EV_ABS)
    {
        if (tc_ev.code == ABS_Y)
        {
            data->point.y = tc_ev.value;
            // lv_event_send(lv_indev_get_obj_act(), LV_EVENT_VALUE_CHANGED, NULL);
        }
        if (tc_ev.code == ABS_X)
        {
            data->point.x = tc_ev.value;
            // lv_event_send(lv_indev_get_obj_act(), LV_EVENT_VALUE_CHANGED, NULL);
        }
        if (pressed == 1)
        {
            data->state = LV_INDEV_STATE_PR;
            lv_event_send(lv_indev_get_obj_act(), LV_EVENT_PRESSING, NULL);
        }
    }
    // printf("state: %d\n", data->state);
    // printf("x: %d\n", data->point.x);
    // printf("y: %d\n", data->point.y);
    // printf("continue: %d\n\n", data->continue_reading);
}

// 不支持drag?
static void drag_event_handler(lv_event_t *e)
{
    pthread_mutex_lock(&lvgl_mutex);
    lv_obj_t *obj = lv_event_get_target(e);

    lv_point_t vect;
    lv_indev_get_vect(indev_touchpad, &vect);

    lv_coord_t x = lv_obj_get_x(obj) + vect.x;
    lv_coord_t y = lv_obj_get_y(obj) + vect.y;
    printf("x: %d\n", x);
    printf("y: %d\n", y);
    lv_obj_set_pos(obj, x, y);
    pthread_mutex_unlock(&lvgl_mutex);
}

static void sw_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *sw = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_obj_t *list = lv_event_get_user_data(e);

        if (lv_obj_has_state(sw, LV_STATE_CHECKED))
            lv_obj_add_flag(list, LV_OBJ_FLAG_SCROLL_ONE);
        else
            lv_obj_clear_flag(list, LV_OBJ_FLAG_SCROLL_ONE);
    }
}

void lv_example_scroll_2(void)
{
    lv_obj_t *panel = lv_obj_create(lv_scr_act());
    lv_obj_set_size(panel, 280, 120);
    lv_obj_set_scroll_snap_x(panel, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
    lv_obj_align(panel, LV_ALIGN_CENTER, 0, 20);

    uint32_t i;
    for (i = 0; i < 10; i++)
    {
        lv_obj_t *btn = lv_btn_create(panel);
        lv_obj_set_size(btn, 150, lv_pct(100));
        // lv_obj_add_flag(btn, LV_OBJ_FLAG_EVENT_BUBBLE); //?

        lv_obj_t *label = lv_label_create(btn);
        if (i == 3)
        {
            lv_label_set_text_fmt(label, "Panel %" LV_PRIu32 "\nno snap", i);
            lv_obj_clear_flag(btn, LV_OBJ_FLAG_SNAPPABLE);
        }
        else
        {
            lv_label_set_text_fmt(label, "Panel %" LV_PRIu32, i);
        }

        lv_obj_center(label);
    }
    lv_obj_update_snap(panel, LV_ANIM_ON);

#if LV_USE_SWITCH
    /*Switch between "One scroll" and "Normal scroll" mode*/
    lv_obj_t *sw = lv_switch_create(lv_scr_act());
    lv_obj_align(sw, LV_ALIGN_TOP_RIGHT, -20, 10);
    lv_obj_add_event_cb(sw, sw_event_cb, LV_EVENT_ALL, panel);
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "One scroll");
    lv_obj_align_to(label, sw, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
#endif
}