#include "bsp.h"
#include "os_wrapper.h"
#include "hwdef.h"
#include "esp_log.h"

constexpr static const char* TAG = "BSP";
BSP bsp;

int BSP::ePaperDisplayInit() {
    EPaperDisplay::SpiConfig cfg = {
        .cs = hwdef::EPD_CS_PIN,
        .dc = hwdef::EPD_DC_PIN,
        .rst = hwdef::EPD_RST_PIN,
        .busy = hwdef::EPD_BUSY_PIN,
        .spi_host = hwdef::EPD_SPI_HOST
    };
    display = new EPaperDisplay(hwdef::EPD_WIDTH, hwdef::EPD_HEIGHT, cfg);
    display->init();
    display->clear();
    display->displayPartBaseImage();
    display->initPartial();
    ESP_LOGI(TAG, "epaper inited");
    return 0;
}

int BSP::boardPowerInit() {
    power = new BoardPower(hwdef::EPD_PWR_PIN, hwdef::AUDIO_PWR_PIN, hwdef::VBAT_PWR_PIN);
    power->epdPowerOn();
    power->vbatPowerOn();
    power->audioPowerOn();
    ESP_LOGI(TAG, "board power inited");
    return 0;
}

int BSP::buttonInit() {
    Wrapper::GPI *boot_gpio = new Wrapper::GPI(hwdef::BOOT_BUTTON_PIN, true);
    Wrapper::GPI *power_gpio = new Wrapper::GPI(hwdef::PWR_BUTTON_PIN, true);
    bootButton = new MultiButton(*boot_gpio, 0, 30, 300);
    powerButton = new MultiButton(*power_gpio, 0, 30, 300);
    ESP_LOGI(TAG, "button inited");
    return 0;
}

int BSP::init() {
    Wrapper::SPI::init(hwdef::EPD_SPI_HOST, hwdef::EPD_MOSI_PIN, -1, hwdef::EPD_SCK_PIN, true, hwdef::EPD_WIDTH * hwdef::EPD_HEIGHT);
    Wrapper::I2C::init(hwdef::I2C_HOST, hwdef::I2C_SDA_PIN, hwdef::I2C_SCL_PIN);
    boardPowerInit();
    buttonInit();
    ePaperDisplayInit();
    return 0;
}

int BSP::deinit() {
    return 0;
}

