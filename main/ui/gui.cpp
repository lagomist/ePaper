#include "gui.h"
#include "bsp.h"
#include "hwdef.h"
#include "esp_log.h"

constexpr static const char TAG[] = "gui";
GUI gui;

void GUI::disp_flush_cb(LVDisplay * disp, const lv_area_t * area, uint8_t * px_map) {
    // I1 format: skip 8-byte palette
    uint8_t * bitmap = px_map + 8;   

    int area_w = lv_area_get_width(area);
    int stride = (area_w + 7) / 8;

    for (int y = area->y1; y <= area->y2; y++) {
        for (int x = area->x1; x <= area->x2; x++) {
            int local_x = x - area->x1;
            int local_y = y - area->y1;
            int byte_idx = local_y * stride + (local_x / 8);
            int bit = 7 - (local_x % 8);
            uint8_t pixel = (bitmap[byte_idx] >> bit) & 0x01;

            uint8_t color = (pixel == 0) ? EPaperDisplay::DRIVER_COLOR_BLACK
                                         : EPaperDisplay::DRIVER_COLOR_WHITE;
            bsp.display->drawColorPixel(x, y, color);
        }
    }

    bsp.display->displayPart();
}

void GUI::init() {
    ESP_LOGI(TAG, "init");
    size_t buf_size = hwdef::EPD_WIDTH * hwdef::EPD_HEIGHT / 8 + 8;
    display.init(hwdef::EPD_WIDTH, hwdef::EPD_HEIGHT, buf_size, 2);
    display.setFlushCallback(disp_flush_cb);
	
    LVDisplay::mutex->lock();
    lv_display_set_color_format(display.getDispHandle(), LV_COLOR_FORMAT_I1);
	// lv_theme_mono_init(0, false, nullptr);
    // lv_theme_set_parent( lv_theme_mono_get(), lv_display_get_theme(NULL));
    /*Create a white label, set its text and align it to the center*/
    lv_obj_t * label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello shabi");
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    LVDisplay::mutex->unlock();
}
