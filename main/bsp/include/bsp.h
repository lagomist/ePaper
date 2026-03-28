#pragma once

#include "epaper.h"
#include "shtc3.h"
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
private:
    int ePaperDisplayInit();
    int boardPowerInit();
    int buttonInit();
};

extern BSP bsp;
