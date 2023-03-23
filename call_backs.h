#pragma once

#include "main.h"
#include "lvgl/lvgl.h"

// #include <math.h>

static int pressed;

static void *tick_thread(void *args)
{
    while (1)
    {
        usleep(20 * 1000); /*Sleep for 5 millisecond*/
        lv_tick_inc(20);   /*Tell LVGL that 5 milliseconds were elapsed*/
        // lv_timer_handler();
        // lv_timer_handler();
        // timer_start();     /*Restart the timer where lv_tick_inc() is called*/
        // lv_task_handler(); /*Call `lv_task_handler()` manually to process the wake-up event*/
        // lv_obj_update_layout(lv_scr_act());
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

    ssize_t n = read(tc_fd, &tc_ev, sizeof(tc_ev));
    // ssize_t n = read(fd, ev, sizeof(struct input_event) * MAX_EVENTS);

    // printf("type: %d ", tc_ev.type);
    // printf("code: %d ", tc_ev.code);
    // printf("value: %d\n\n", tc_ev.value);
    // if (n < sizeof(tc_ev))
    // {
    //     return;
    // }
    if (tc_ev.type == 0)
    {
        if (pressed == 1)
        {
            data->state = LV_INDEV_STATE_PR;
        }
        else
        {
            data->state = LV_INDEV_STATE_REL;
        }
    }
    if (tc_ev.type == EV_KEY)
    {
        if (tc_ev.code == BTN_TOUCH)
        {
            if (tc_ev.value == 1)
            {
                pressed = 1;
                data->state = LV_INDEV_STATE_PR;
            }
            else
            {
                pressed = 0;
                data->state = LV_INDEV_STATE_REL;
            }
        }
    }
    else if (tc_ev.type == EV_ABS)
    {
        if (tc_ev.code == ABS_Y)
        {
            data->point.y = (tc_ev.value * 800 + 512) >> 10;
            if (pressed == 1)
            {
                data->state = LV_INDEV_STATE_PR;
            }
            // lv_event_send(lv_indev_get_obj_act(), LV_EVENT_VALUE_CHANGED, NULL);
        }
        if (tc_ev.code == ABS_X)
        {
            data->point.x = (tc_ev.value * 800 + 512) >> 10;
            if (pressed == 1)
            {
                data->state = LV_INDEV_STATE_PR;
            }
            // lv_event_send(lv_indev_get_obj_act(), LV_EVENT_VALUE_CHANGED, NULL);
        }
    }
    // printf("state: %d\n", data->state);
    // printf("x: %d\n", data->point.x);
    // printf("y: %d\n", data->point.y);
    // printf("continue: %d\n\n", data->continue_reading);
    // data->continue_reading = true;
}

// static void *render_flush(void *args)
// {
//     usleep(40 * 1000); /* delay 5ms to avoid unnecessary polling */
//     _lv_disp_refr_timer(NULL);
// }

static void drag_event_handler(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);

    lv_indev_t *indev = lv_indev_get_act();
    if (indev == NULL)
        return;

    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    lv_coord_t x = lv_obj_get_x(obj) + vect.x;
    lv_coord_t y = lv_obj_get_y(obj) + vect.y;
    lv_obj_set_pos(obj, x, y);
}

/**
 * Make an object dragable.
 */
void lv_example_obj_2(lv_obj_t *anc)
{
    lv_obj_t *obj;
    obj = lv_obj_create(anc);
    lv_obj_set_size(obj, 150, 100);
    lv_obj_add_event_cb(obj, drag_event_handler, LV_EVENT_PRESSING, NULL);

    lv_obj_t *label = lv_label_create(obj);
    lv_label_set_text(label, "Drag me");
    lv_obj_center(label);
}

static void anim_x_cb(void *var, int32_t v)
{
    lv_obj_set_x(var, v);
}

static void sw_event_cb(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    lv_obj_t *label = lv_event_get_user_data(e);

    if (lv_obj_has_state(sw, LV_STATE_CHECKED))
    {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, label);
        lv_anim_set_values(&a, lv_obj_get_x(label), 100);
        lv_anim_set_time(&a, 500);
        lv_anim_set_exec_cb(&a, anim_x_cb);
        lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
        lv_anim_start(&a);
    }
    else
    {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, label);
        lv_anim_set_values(&a, lv_obj_get_x(label), -lv_obj_get_width(label));
        lv_anim_set_time(&a, 500);
        lv_anim_set_exec_cb(&a, anim_x_cb);
        lv_anim_set_path_cb(&a, lv_anim_path_ease_in);
        lv_anim_start(&a);
    }
}

/**
 * Start animation on an event
 */
void lv_example_anim_1(lv_obj_t *anc)
{
    lv_obj_t *label = lv_label_create(anc);
    lv_label_set_text(label, "Hello animations!");
    lv_obj_set_pos(label, 100, 10);

    lv_obj_t *sw = lv_switch_create(anc);
    lv_obj_center(sw);
    lv_obj_add_state(sw, LV_STATE_CHECKED);
    lv_obj_add_event_cb(sw, sw_event_cb, LV_EVENT_VALUE_CHANGED, label);
}

// void lv_example_monkey_1(void)
// {
//     /*Create pointer monkey test*/
//     lv_monkey_config_t config;
//     lv_monkey_config_init(&config);
//     config.type = LV_INDEV_TYPE_KEYPAD;
//     config.period_range.min = 40;
//     config.period_range.max = 400;
//     lv_monkey_t * monkey = lv_monkey_create(&config);

//     /*Start monkey test*/
//     lv_monkey_set_enable(monkey, true);
// }