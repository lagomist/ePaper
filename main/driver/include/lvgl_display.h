#pragma once

#include "os_wrapper.h"
#include "lvgl.h"
#include <cstdint>

class LVDisplay {
public:
    using FlushCallback = void(*)(LVDisplay * disp_drv, const lv_area_t * area, uint8_t * px_map);

    void init(uint16_t hor_res, uint16_t ver_res, size_t buf_size, uint8_t num_fb);
    void setFlushCallback(FlushCallback cb) { _flush_cb = cb; }
    lv_display_t *getDispHandle() const { return disp; }

    static void lvgl_init(void);
    static Wrapper::OS::RecursiveMutex *mutex;
    static constexpr uint32_t LV_TICK_PERIOD_MS = 5;

private:
    static void gui_disp_task(void *pvParameter);
    static void disp_flush_cb(lv_display_t * drv, const lv_area_t * area, uint8_t * px_map);

    FlushCallback _flush_cb = nullptr;
    Wrapper::OS::Task _thread;
    lv_display_t * disp = nullptr;
};


class UiPage {
public:
    using entry = void(*)(void *arg);
    UiPage(const char* name, entry show, entry hide): _name(name), _show(show), _hide(hide) {}
    ~UiPage() {}
    void show(void *arg = nullptr) { _show(arg); }
    void hide(void *arg = nullptr) { _hide(arg); }
private:
    const char *_name;
    entry _show;
    entry _hide;
};


