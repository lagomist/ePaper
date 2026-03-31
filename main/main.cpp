#include "nvs_wrapper.h"
#include "fs_wrapper.h"
#include "bsp.h"
#include "button_handler.h"
#include "gui.h"


extern "C" void app_main(void) {
    Wrapper::NVS::init("nvs");
    Wrapper::FileSystem::mount();

    bsp.init();
    buttonMonitor.init();
    gui.init();
}
