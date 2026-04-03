#include "nvs_wrapper.h"
#include "fs_wrapper.h"
#include "wifi_wrapper.h"
#include "bsp.h"
#include "button_handler.h"
#include "led_heartbeat.h"
#include "gui.h"


extern "C" void app_main(void) {
    Wrapper::NVS::init("nvs");
    Wrapper::FileSystem::Flash::mount();
    Wrapper::WiFi::netif_init();
    Wrapper::WiFi::Station::init();

    bsp.init();
    ledHeartbeat.init(*bsp.led);
    buttonMonitor.init();
    gui.init();
}
