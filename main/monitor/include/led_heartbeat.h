#pragma once


#include "os_wrapper.h"
#include "gpio_wrapper.h"

class LedHeartbeat {
public:
    void init(Wrapper::GPOBase &led);
    void start() { enable = true; }
    void stop() { enable = false; }
private:
    bool enable = true;
    static void ledHandler(void *arg);
    Wrapper::GPOBase *dev = nullptr;
    Wrapper::OS::Task *thread = nullptr;
};

extern LedHeartbeat ledHeartbeat;

