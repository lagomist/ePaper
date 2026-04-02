#pragma once

#include "epaper.h"
#include "shtc3.h"
#include "gpio_wrapper.h"
#include "multi_button.h"
#include "board_power.h"
#include "board_audio.h"

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
    BoardAudio *audio;
private:
    int ePaperDisplayInit();
    int audioInit();
    int boardPowerInit();
    int buttonInit();
    int ledInit();
};

extern BSP bsp;
