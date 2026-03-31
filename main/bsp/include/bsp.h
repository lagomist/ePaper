#pragma once

#include "epaper.h"
#include "shtc3.h"
#include "gpio_wrapper.h"
#include "multi_button.h"
#include "board_power.h"

class BSP {
public:
    BSP() {}
    ~BSP() {}
    int init();
    int deinit();

    EPaperDisplay *display;
    BoardPower *power;
    Shtc3 *tempHumi;
    MultiButton *powerButton;
    MultiButton *bootButton;
    Wrapper::GPOBase *led;
private:
    int ePaperDisplayInit();
    int boardPowerInit();
    int buttonInit();
    int ledInit();
};

extern BSP bsp;
