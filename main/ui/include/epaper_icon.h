#ifndef EPAPER_ICON_H
#define EPAPER_ICON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Each icon is 16x16px, 1 bit per pixel (row-major, MSB first in each byte): 32 bytes per icon
extern const uint8_t icon_sdcard_16x16[32];
extern const uint8_t icon_power_16x16[32];
extern const uint8_t icon_rtc_16x16[32];
extern const uint8_t icon_shtc3_16x16[32];
extern const uint8_t icon_button_16x16[32];
extern const uint8_t icon_led_16x16[32];
extern const uint8_t icon_mic_16x16[32];
extern const uint8_t icon_mcu_16x16[32];

// Draw bitmap icon at x,y on ePaper display
void epaper_draw_icon16(int x, int y, const uint8_t icon[32]);

#ifdef __cplusplus
}
#endif

#endif // EPAPER_ICON_H
