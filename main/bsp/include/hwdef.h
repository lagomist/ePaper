#pragma once

#include <cstddef>
#include <cstdint>

namespace hwdef { 

/* EPD port */
constexpr int EPD_WIDTH         = 200;
constexpr int EPD_HEIGHT        = 200;
constexpr uint8_t EPD_SPI_HOST  = 1;

constexpr int EPD_DC_PIN        = 10;
constexpr int EPD_CS_PIN        = 11;
constexpr int EPD_SCK_PIN       = 12;
constexpr int EPD_MOSI_PIN      = 13;
constexpr int EPD_RST_PIN       = 9;
constexpr int EPD_BUSY_PIN      = 8;

/* DEV POWER */
constexpr int EPD_PWR_PIN       = 6;
constexpr int AUDIO_PWR_PIN     = 42;
constexpr int VBAT_PWR_PIN      = 17;

/* Button */
constexpr int BOOT_BUTTON_PIN   = 0;
constexpr int PWR_BUTTON_PIN    = 18;

/* I2C */
constexpr uint8_t I2C_HOST      = 0;
constexpr int I2C_SDA_PIN       = 47;
constexpr int I2C_SCL_PIN       = 48;

}
