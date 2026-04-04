#pragma once

#include "epaper.h"
#include "shtc3.h"
#include "gpio_wrapper.h"
#include "multi_button.h"
#include "board_power.h"
#include "board_audio.h"
#include "board_rtc.h"
#include "vbat_adc.h"

class BSP {
public:
    BSP() {}
    ~BSP() {}
    int init();
    int deinit();

    EPaperDisplay *display;
    BoardPower *power;
    Shtc3 *tempHumi;
    BoardRTC *rtc;
    MultiButton *powerButton;
    MultiButton *bootButton;
    Wrapper::GPOBase *led;
    BoardAudio *audio;
    VbatADC *vbat;
private:
    int ePaperDisplayInit();
    int audioInit();
    int boardPowerInit();
    int buttonInit();
    int hardwareInit();
};

extern BSP bsp;
