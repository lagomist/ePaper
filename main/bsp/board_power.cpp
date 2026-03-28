#include "board_power.h"

BoardPower::BoardPower(int epd_power_pin, int audio_power_pin, int vbat_power_pin) :
    epd_power(epd_power_pin),
    audio_power(audio_power_pin),
    vbat_power(vbat_power_pin) {
    
}

BoardPower::~BoardPower() {

}

void BoardPower::epdPowerOn() {
    epd_power.set(0);
}

void BoardPower::epdPowerOff() {
    epd_power.set(1);
}

void BoardPower::audioPowerOn() {
    audio_power.set(0);
}

void BoardPower::audioPowerOff() {
    audio_power.set(1);
}

void BoardPower::vbatPowerOn() {
    vbat_power.set(1);
}

void BoardPower::vbatPowerOff() {
    vbat_power.set(0);
}
