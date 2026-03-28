#include "dashboard_widget.h"
#include "epaper_icon.h"
#include <cstdio>
// 建议包含具体 driver 头: #include "driver/sdmmc_host.h", "driver/adc.h", "driver/rtc_io.h", "driver/gpio.h", "driver/i2c.h", 等

const uint8_t* SDCardWidget::getIcon() const { return icon_sdcard_16x16; }
void SDCardWidget::update() {
    // SD卡挂载检测（请替换为对应 FS/SD driver 实例判定）
    // present = esp_sdcard_is_mounted()/custom_fs_mounted();
    present = true; // TODO: 替换为实际SD状态
}
std::string SDCardWidget::getStatusText() const {
    return present ? "SD: OK" : "SD: ERR";
}

const uint8_t* PowerWidget::getIcon() const { return icon_power_16x16; }
void PowerWidget::update() {
    // 电池电压采样，普遍为 ADC GPIO 通道
    // voltage = adc1_get_voltage(ADC1_CHANNEL_0) * 比例系数;
    voltage = 3.70f; // TODO: 替换为实际电压
}
std::string PowerWidget::getStatusText() const {
    char buf[16]; snprintf(buf, sizeof(buf), "%.2fV", voltage); return std::string(buf);
}

const uint8_t* RTCWidget::getIcon() const { return icon_rtc_16x16; }
void RTCWidget::update() {
    // RTC 实时时钟（i2c/rtc库，示例判断是否走时）
    // running = rtc_check_time_valid()/rtc_is_running();
    running = true; // TODO: 替换为RTC运行状态
}
std::string RTCWidget::getStatusText() const {
    return running ? "RTC: Tick" : "RTC: ERR";
}

const uint8_t* SHTC3Widget::getIcon() const { return icon_shtc3_16x16; }
void SHTC3Widget::update() {
    // I2C温湿度，调用 shtc3_read()/sht3x_read()/bsp driver
    // float t, h; if (read_shtc3(&t, &h) == 0) { temp=t; humidity=h; }
    temp = 25.1f; humidity = 42.0f; // TODO: 替换为真实读数
}
std::string SHTC3Widget::getStatusText() const {
    char buf[16]; snprintf(buf, sizeof(buf), "T%.1f H%.0f", temp, humidity); return std::string(buf);
}

const uint8_t* ButtonWidget::getIcon() const { return icon_button_16x16; }
void ButtonWidget::update() {
    // 按钮实时状态（可绑定主控按钮）
    // pressed = (gpio_get_level(GPIO_NUM_XX) == 0);
    pressed = false; // TODO: 绑定真实按钮
}
std::string ButtonWidget::getStatusText() const {
    return pressed ? "BTN: ON" : "BTN: OFF";
}

const uint8_t* LEDWidget::getIcon() const { return icon_led_16x16; }
void LEDWidget::update() {
    // 查询LED IO状态或PWM寄存器（或直接用一全局变量绑定实际LED）
    // led_on = (gpio_get_level(GPIO_NUM_LED) == 1)
    led_on = true; // TODO: 替换为真实LED状态
}
std::string LEDWidget::getStatusText() const {
    return led_on ? "LED: ON" : "LED: OFF";
}

const uint8_t* MICWidget::getIcon() const { return icon_mic_16x16; }
void MICWidget::update() {
    // MIC如为ADC/音频通道，取短时均值即可
    // level = read_mic_adc_average();
    level = 0.3f; // TODO: 替换为音量/MIC阈值
}
std::string MICWidget::getStatusText() const {
    char buf[16]; snprintf(buf, sizeof(buf), "MIC: %.1f", level); return std::string(buf);
}

const uint8_t* MCUWidget::getIcon() const { return icon_mcu_16x16; }
void MCUWidget::update() {
    // MCU自检（软件心跳、温度、电源等自感知）
    // ok = check_selftest_pass();
    ok = true; // TODO: 替换为MCU健康标志
}
std::string MCUWidget::getStatusText() const {
    return ok ? "MCU: OK" : "MCU: ERR";
}
