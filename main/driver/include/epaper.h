#pragma once

#include "gpio_wrapper.h"
#include "spi_wrapper.h"
#include <cstdint>


class EPaperDisplay {
public:
    /* Display color */
    enum ColorImage {
        DRIVER_COLOR_WHITE  = 0xff,
        DRIVER_COLOR_BLACK  = 0x00,
        FONT_BACKGROUND = DRIVER_COLOR_WHITE,
    };
    struct SpiConfig {
        int cs;
        int dc;
        int rst;
        int busy;
        uint8_t spi_host;
    };
    EPaperDisplay(int width, int height, SpiConfig cfg);
    ~EPaperDisplay();

    /* 墨水屏初始化 */
    void init();
    /* 清空屏幕 */
    void clear();   
    void display(); /* 刷buffer到墨水屏 */
    
    /*局部刷新*/
    void displayPartBaseImage();
    void initPartial();
    void displayPart();
    void drawColorPixel(uint16_t x, uint16_t y, uint8_t color);

private:
    struct init_cmd_t {
        uint8_t cmd;
        uint8_t data[16];
        uint8_t databytes; // No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
    };
    void sendCommand(const uint8_t command, const uint8_t buffer[], size_t len);
    void writeBytes(const uint8_t buffer[], size_t len);
    void wait_busy();

    void setWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
    void setCursor(uint16_t Xstart, uint16_t Ystart);
    void setLut(const uint8_t *lut);
    void turnOnDisplay();
    void turnOnDisplayPart();

    const int _width;
    const int _height;
    const size_t _buffer_len;
    uint8_t *_buffer = nullptr;

    Wrapper::GPO _rst;
    Wrapper::GPO _dc;
    Wrapper::GPI _busy;
    Wrapper::SPI::Device _bus;
};
