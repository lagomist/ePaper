#include "led_heartbeat.h"
#include "esp_log.h"

constexpr static const char *TAG = "LedHeartbeat";

LedHeartbeat ledHeartbeat;

void LedHeartbeat::ledHandler(void *arg) {
    LedHeartbeat *self = (LedHeartbeat *)arg;
    while (1) {
        if (!self->enable) {
            Wrapper::OS::delay(100);
            continue;
        }

        self->dev->set(0);
        Wrapper::OS::delay(150);
        self->dev->set(1);
        Wrapper::OS::delay(150);

        self->dev->set(0);
        Wrapper::OS::delay(150);
        self->dev->set(1);
        Wrapper::OS::delay(500);
    }
    
}

void LedHeartbeat::init(Wrapper::GPOBase &led) {
    dev = &led;
    thread = new Wrapper::OS::Task();
    thread->create(ledHandler, this, "heartbeat", 4 * 1024, 2);
}
