#include "main.h"
#include "lvgl/lvgl.h"

static lv_disp_t *disp;

static lv_disp_drv_t disp_drv; /*A variable to hold the drivers. Must be static or global.*/
static lv_disp_draw_buf_t disp_buf;
static lv_color_t buf1[SCREEN_WIDTH * SCREEN_HEIGHT], buf2[SCREEN_WIDTH * SCREEN_HEIGHT];

static lv_indev_drv_t indev_drv;
static lv_indev_t *indev_touchpad;

static lv_fs_dir_t dir;
static lv_fs_res_t res;
static char files[256][256];
static char path[266];

#include "call_backs.h"
// #include "layout/base_layout.h"
// #include "layout/content_section.h"

void open_path()
{
    res = lv_fs_dir_open(&dir, "S:/images");
    if (res == LV_FS_RES_HW_ERR)
    {
        perror("1");
    }
    if (res == LV_FS_RES_NOT_IMP)
    {
        perror("2");
    }
    if (res == LV_FS_RES_UNKNOWN)
    {
        perror("3");
    }

    if (res != LV_FS_RES_OK)
    {
        perror("can't open the path\n");
        // exit(0x21);
    }
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
        if (res != LV_FS_RES_OK)
        {
            printf("read files error at: %d\n", count);
            // exit(0x22);
        }
        /*fn is empty, if not more files to read*/
        if (strlen(fn) == 0)
        {
            break;
        }
        strcpy(files[count], fn);
        printf("%s\n", files[count]);
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
    disp_drv.full_refresh = 1;

    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, SCREEN_WIDTH * SCREEN_HEIGHT);

    disp_drv.draw_buf = &disp_buf; /*Set an initialized buffer*/
    disp_drv.flush_cb = flush_cb;  /*Set a flush callback to draw to the display*/
    disp_drv.hor_res = vinfo.xres; /*Set the horizontal resolution in pixels*/
    disp_drv.ver_res = vinfo.yres; /*Set the vertical resolution in pixels*/

    disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/

    // Register the touch plane
    lv_indev_drv_init(&indev_drv); /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read;
    /*Register the driver in LVGL and save the created input device object*/
    indev_touchpad = lv_indev_drv_register(&indev_drv);

    lv_group_t *group = lv_group_create();
    lv_group_add_obj(group, lv_scr_act());
    lv_indev_set_group(indev_touchpad, group);

    int res = pthread_create(&mythread1, NULL, tick_thread, NULL);
    if (res != 0)
    {
        printf("thread create faild! \n");
        // exit(0x20);
    }

    // res = pthread_create(&render, NULL, render_flush, NULL);
    // if (res != 0)
    // {
    //     printf("thread create faild! \n");
    //     // exit(0x20);
    // }

    // todo: file system
    open_path();
    read_all_files();

    lv_obj_t *my_img;
    my_img = lv_img_create(lv_scr_act());
    sprintf(path,"S:/images/%s\0",files[0]);
    printf("%s\n",path);

    lv_img_set_src(my_img, path);


    //--------------------- UI -----------------------//
    // //?style here
    // init_base_style();

    // //?layout here
    // base_layout();
    // test_section();
    // bing_wallpaper_CyprusMaze_1920x1080.jpg
    //--------------------- UI -----------------------//

    while (1)
    {
        lv_timer_handler_run_in_period(20);
        usleep(20 * 1000); /*Sleep for 5 millisecond*/
    }

    close_path();
    close_touch();
    close_framebuffer_device(screensize);
    return 0;
}