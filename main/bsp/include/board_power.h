#pragma once

#include "gpio_wrapper.h"

class BoardPower {
private:
    Wrapper::GPO epd_power;
    Wrapper::GPO audio_power;
    Wrapper::GPO vbat_power;

public:
    BoardPower(int epd_power_pin, int audio_power_pin, int vbat_power_pin);
    ~BoardPower();

    void epdPowerOn();
    void epdPowerOff();
    void audioPowerOn();
    void audioPowerOff();
    void vbatPowerOn();
    void vbatPowerOff();
};
