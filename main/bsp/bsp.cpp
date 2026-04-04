#include "bsp.h"
#include "os_wrapper.h"
#include "fs_wrapper.h"
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

int BSP::audioInit() {
    audio = new BoardAudio();
    // in_out: {codec: ES8311, pa: 46, use_mclk: 1, pa_gain:6}
    BoardAudio::I2cConfig i2c_cfg = {};
    i2c_cfg.port = hwdef::AUDIO_I2C_HOST;
    i2c_cfg.scl = hwdef::AUDIO_I2C_SCL;
    i2c_cfg.sda = hwdef::AUDIO_I2C_SDA;
    BoardAudio::I2sConfig i2s_cfg = {};
    i2s_cfg.mclk = hwdef::AUDIO_I2S_MCLK;
    i2s_cfg.bclk = hwdef::AUDIO_I2S_BCLK;
    i2s_cfg.ws = hwdef::AUDIO_I2S_WS;
    i2s_cfg.dout = hwdef::AUDIO_I2S_DOUT;
    i2s_cfg.din = hwdef::AUDIO_I2S_DIN;
    i2s_cfg.port = hwdef::AUDIO_I2S_PORT;
    BoardAudio::CodecCfg codec_cfg = {};
    codec_cfg.mode = BoardAudio::I2sMode::CODEC_STD;
    codec_cfg.pa_gain = 6;
    codec_cfg.pa_pin = hwdef::AUDIO_PA_PIN;
    codec_cfg.use_mclk = true;

    audio->init(i2c_cfg, i2s_cfg, codec_cfg);
    audio->playerInit();
    ESP_LOGI(TAG, "audio ES8311 inited");
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

int BSP::hardwareInit() {
    led = new Wrapper::GPO(hwdef::LED_GREEN_PIN, 1);
    tempHumi = new Shtc3(hwdef::I2C_HOST);
    rtc = new BoardRTC(hwdef::I2C_HOST);
    vbat = new VbatADC(hwdef::VBAT_ADC_CHANNEL, 0);
    ESP_LOGI(TAG, "hardware inited");
    return 0;
}


int BSP::init() {
    Wrapper::SPI::init(hwdef::EPD_SPI_HOST, hwdef::EPD_MOSI_PIN, -1, hwdef::EPD_SCK_PIN, true, hwdef::EPD_WIDTH * hwdef::EPD_HEIGHT);
    Wrapper::I2C::init(hwdef::I2C_HOST, hwdef::I2C_SDA_PIN, hwdef::I2C_SCL_PIN);
    Wrapper::FileSystem::SdCard::mount(hwdef::SD_CARD_SCK_PIN, hwdef::SD_CARD_MOSI_PIN, hwdef::SD_CARD_MISO_PIN);
    boardPowerInit();
    buttonInit();
    hardwareInit();
    ePaperDisplayInit();
    audioInit();
    return 0;
}

int BSP::deinit() {
    return 0;
}

