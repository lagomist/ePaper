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

/* LED */
constexpr int LED_GREEN_PIN     = 3;

/* Audio*/
constexpr int AUDIO_PA_PIN          = 46;

constexpr uint8_t AUDIO_I2C_HOST    = I2C_HOST;
constexpr int AUDIO_I2C_SDA         = I2C_SDA_PIN;
constexpr int AUDIO_I2C_SCL         = I2C_SCL_PIN;

constexpr int AUDIO_I2S_PORT        = 2;
constexpr int AUDIO_I2S_BCLK        = 15;
constexpr int AUDIO_I2S_WS          = 38;
constexpr int AUDIO_I2S_DOUT        = 45;
constexpr int AUDIO_I2S_DIN         = 16;
constexpr int AUDIO_I2S_MCLK        = 14;

/* Vbat ADC */
constexpr uint8_t VBAT_ADC_CHANNEL  = 3;

/* SD card */
constexpr int SD_CARD_SCK_PIN    = 39;
constexpr int SD_CARD_MOSI_PIN   = 41;
constexpr int SD_CARD_MISO_PIN   = 40;

}
