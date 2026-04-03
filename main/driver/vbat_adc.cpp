#include "vbat_adc.h"

VbatADC::VbatADC(uint8_t channel, uint8_t unit)
: Wrapper::ADCOneshot(channel, unit, ADC_ATTEN_DB_12) {
}

VbatADC::~VbatADC() {

}

float VbatADC::getVbatVoltage() {
    int volt = getVoltage();
    float VbatVoltage = 0.001 * volt * 2;
    return VbatVoltage;
}

uint8_t VbatADC::getBatteryLevel() {
    float vol = getVbatVoltage();
    if(vol < 3.0) {
        return 0;
    }
    if(vol > 4.12) {
        return 100;
    }
    // 1.12 = 4.12 - 3.0
    float level = ((vol-3.0) / 1.12) * 100;   
    return (uint8_t)level;
}
