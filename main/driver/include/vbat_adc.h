#ifndef ADC_BSP_H
#define ADC_BSP_H

#include "adc_wrapper.h"
#include <cstdint>

class VbatADC : public Wrapper::ADCOneshot {
public:
    VbatADC(uint8_t channel, uint8_t unit = 0);
    ~VbatADC();

    float getVbatVoltage();
    uint8_t getBatteryLevel();
private:
};

#endif
