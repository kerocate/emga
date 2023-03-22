#pragma once

#define _GNU_SOURCE

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/fb.h>
#include <linux/input.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int lcdfd = -1;
static uint32_t *fbp = NULL;

static int tc_fd = -1;
struct input_event tc_ev;

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

// static pthread_mutex_t lvgl_mutex;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

static size_t open_framebuffer_device(void)
{
    int fd = open("/dev/fb0", O_RDWR);
    if (fd == -1)
    {
        perror("Error: cannot open framebuffer device");
        // exit(0x10);
    }
    ioctl(fd, FIONBIO, 1);
    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        perror("Error: cannot get variable screen info");
        close(fd);
        // exit(0x11);
    }

    size_t screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    fbp = mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (fbp == MAP_FAILED)
    {
        perror("Error: failed to map framebuffer");
        close(fd);
        // exit(0x12);
    }

    lcdfd = fd;
    return screensize;
}

static void close_framebuffer_device(size_t screensize)
{
    munmap(fbp, screensize);
    close(lcdfd);
}

static void open_touch()
{
    tc_fd = open("/dev/input/event0", O_RDWR);
    if (tc_fd < 0)
    {
        printf("open touch fail!\n");
        // exit(0x13);
    }
    ioctl(tc_fd, FIONBIO, 1);
}

static void close_touch(){
    close(tc_fd);
}