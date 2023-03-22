#include "main.h"
#include "lvgl/lvgl.h"

static lv_disp_drv_t disp_drv; /*A variable to hold the drivers. Must be static or global.*/
static lv_disp_draw_buf_t disp_buf;
// static lv_color_t buf1[SCREEN_WIDTH * 10];
static lv_color_t buf1[SCREEN_WIDTH * SCREEN_HEIGHT];
static lv_color_t buf2[SCREEN_WIDTH * 10];

static lv_indev_drv_t indev_drv;
lv_indev_t *indev_touchpad;

static lv_fs_dir_t dir;
static lv_fs_res_t res;
static char files[256][256];

#include "call_backs.h"
#include "layout/base.h"

void open_path()
{
    res = lv_fs_dir_open(&dir, "S:/images");
    // if (res != LV_FS_RES_OK)
    // {
    //     perror("can't open the path\n");
    //     // exit(0x21);
    // }
}

void close_path()
{
    lv_fs_dir_close(&dir);
}

void read_all_files()
{
    char fn[256] = {'\0'};
    int count = 0;
    while (1)
    {
        res = lv_fs_dir_read(&dir, fn);
        // if (res != LV_FS_RES_OK)
        // {
        //     printf("read files error at: %d\n", count);
        //     // exit(0x22);
        // }
        /*fn is empty, if not more files to read*/
        if (strlen(fn) == 0)
        {
            break;
        }
        strcpy(files[count], fn);
        printf("%s\n", fn);
        count++;
    }
}

int main(int argc, char const *argv[])
{
    pthread_t mythread1;

    // ?: lvint->lvtick->lvdata->lvdraw
    lv_init();
    size_t screensize = open_framebuffer_device();
    open_touch();

    // Register the display
    /*Initialize `disp_buf` with the buffer(s). With only one buffer use NULL instead buf_2 */
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/
    disp_drv.direct_mode = 1;
    // lv_disp_draw_buf_init(&disp_buf, buf1, buf2, SCREEN_WIDTH * 10);
    lv_disp_draw_buf_init(&disp_buf, buf1, NULL, SCREEN_WIDTH * SCREEN_HEIGHT);
    disp_drv.draw_buf = &disp_buf; /*Set an initialized buffer*/
    disp_drv.flush_cb = flush_cb;  /*Set a flush callback to draw to the display*/
    disp_drv.hor_res = vinfo.xres; /*Set the horizontal resolution in pixels*/
    disp_drv.ver_res = vinfo.yres; /*Set the vertical resolution in pixels*/

    lv_disp_t *disp;
    disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/

    // lv_disp_t *disp = lv_disp_create(vinfo.xres, vinfo.yres);
    // lv_disp_set_flush_cb(disp, flush_cb);
    // lv_disp_set_draw_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISP_RENDER_MODE_PARTIAL);

    // Register the touch plane
    lv_indev_drv_init(&indev_drv); /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read;
    /*Register the driver in LVGL and save the created input device object*/
    indev_touchpad = lv_indev_drv_register(&indev_drv);

    lv_group_t *group = lv_group_create();
    lv_group_add_obj(group, lv_scr_act());
    lv_indev_set_group(indev_touchpad, group);

    // int res = pthread_create(&mythread1, NULL, tick_thread, NULL);
    // if (res != 0)
    // {
    //     printf("thread create faild! \n");
    //     // exit(0x20);
    // }

    // todo: file system
    open_path();
    read_all_files();

    // todo: UI here
    // base_layout();

    // lv_obj_t *cont = lv_obj_create(lv_scr_act());
    // lv_obj_remove_style_all(cont);
    // lv_obj_set_size(cont, 800, 480);
    // lv_obj_center(cont);

    lv_example_scroll_2();

    // lv_obj_t *btn = lv_btn_create(lv_scr_act());
    // lv_obj_add_event(btn, my_event_cb, LV_EVENT_CLICKED, NULL); /*Assign an event callback*/

    while (1)
    {
        usleep(10 * 1000);                  /*Sleep for 5 millisecond*/
        lv_tick_inc(10);                    /*Tell LVGL that 5 milliseconds were elapsed*/
        lv_timer_handler_run_in_period(10); /* run lv_timer_handler() every 5ms */
        // usleep(5 * 1000);                  /* delay 5ms to avoid unnecessary polling */
        // flush_cb(disp,)
        // read(tc_fd, &tc_ev, sizeof(tc_ev));
        // printf("time: %ld ms, type: %d, code: %d, data: %d \n", tc_ev.time.tv_usec, tc_ev.type, tc_ev.code, tc_ev.value);
    }

    close_path();
    close_touch();
    close_framebuffer_device(screensize);
    return 0;
}