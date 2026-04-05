#include "nvs_wrapper.h"
#include "fs_wrapper.h"
#include "wifi_wrapper.h"
#include "bsp.h"
#include "button_handler.h"
#include "led_heartbeat.h"
#include "gui.h"
#include "time_sync.h"


extern "C" void app_main(void) {
    setenv("TZ", "GMT-8", 1);
	tzset();
    Wrapper::NVS::init("nvs");
    Wrapper::FileSystem::Flash::mount();
    Wrapper::WiFi::netif_init();
    Wrapper::WiFi::Station::init();

    bsp.init();
    timeSync.init();
    timeSync.start();
    ledHeartbeat.init(*bsp.led);
    buttonMonitor.init();
    gui.init();
}
