#pragma once

#include "os_wrapper.h"
#include "multi_button.h"

class ButtonMonitor {
public:
    enum Event {
        BOOT_SINGLE_CLICK   = 0x01,
        BOOT_DOUBLE_CLICK   = 0x02,
        BOOT_LONG_PRESS     = 0x04,
        POWER_SINGLE_CLICK  = 0x08,
        POWER_DOUBLE_CLICK  = 0x10,
        POWER_LONG_PRESS    = 0x20,
    };
    void init();
private:
    
    void bootBtnHandler(MultiButton &btn);
    void powerBtnHandler(MultiButton &btn);
    static void btnEventHandler(void *arg);
    Wrapper::OS::EventGroup *_event;
    Wrapper::OS::Timer *_timer;
    Wrapper::OS::Task *_task;
};

extern ButtonMonitor buttonMonitor;

