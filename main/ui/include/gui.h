#pragma once

#include "lvgl_display.h"

class GUI {
public:
    void init();

private:
    static void disp_flush_cb(LVDisplay * disp, const lv_area_t * area, uint8_t * color_p);

    LVDisplay display;
};

extern GUI gui;
