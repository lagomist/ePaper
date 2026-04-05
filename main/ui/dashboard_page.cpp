#include "dashboard_page.h"
#include "bsp.h"
#include "esp_log.h"
#include "lvgl.h"

constexpr static const char* TAG = "DashboardPage";

DashboardPage dashboardPage;

DashboardPage::DashboardPage() : m_root(nullptr), m_update_timer(nullptr) {
}

DashboardPage::~DashboardPage() {
    if (m_update_timer) {
        lv_timer_delete(m_update_timer);
        m_update_timer = nullptr;
    }
}

const char* DashboardPage::getBatteryIcon(uint8_t percent) {
    if (percent <= 20) return LV_SYMBOL_BATTERY_EMPTY;
    if (percent <= 40) return LV_SYMBOL_BATTERY_1;
    if (percent <= 60) return LV_SYMBOL_BATTERY_2;
    if (percent <= 80) return LV_SYMBOL_BATTERY_3;
    return LV_SYMBOL_BATTERY_FULL;
}

void DashboardPage::create(lv_obj_t* parent) {
    m_root = lv_obj_create(parent);
    lv_obj_set_size(m_root, 200, 200);
    lv_obj_align(m_root, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_border_width(m_root, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(m_root, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_pad_all(m_root, 0, LV_PART_MAIN);

    lv_obj_set_flex_flow(m_root, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(m_root, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_BETWEEN);

    lv_obj_t* top_bar = lv_obj_create(m_root);
    lv_obj_set_size(top_bar, 200, 28);
    lv_obj_set_style_border_width(top_bar, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(top_bar, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_pad_hor(top_bar, 4, LV_PART_MAIN);
    lv_obj_set_style_pad_ver(top_bar, 2, LV_PART_MAIN);
    lv_obj_set_flex_flow(top_bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(top_bar, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(top_bar, 2, LV_PART_MAIN);

    lv_obj_t* title = lv_label_create(top_bar);
    lv_label_set_text(title, "Dashboard");
    lv_obj_set_style_text_color(title, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, LV_PART_MAIN);

    m_wifi_icon = lv_label_create(top_bar);
    lv_label_set_text(m_wifi_icon, "");
    lv_obj_set_style_text_color(m_wifi_icon, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_text_font(m_wifi_icon, &lv_font_montserrat_14, LV_PART_MAIN);

    m_sd_icon = lv_label_create(top_bar);
    lv_label_set_text(m_sd_icon, "");
    lv_obj_set_style_text_color(m_sd_icon, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_text_font(m_sd_icon, &lv_font_montserrat_14, LV_PART_MAIN);

    m_batt_icon = lv_label_create(top_bar);
    lv_label_set_text(m_batt_icon, LV_SYMBOL_BATTERY_FULL);
    lv_obj_set_style_text_color(m_batt_icon, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_text_font(m_batt_icon, &lv_font_montserrat_14, LV_PART_MAIN);

    m_batt_percent = lv_label_create(top_bar);
    lv_label_set_text(m_batt_percent, "100%");
    lv_obj_set_style_text_color(m_batt_percent, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_text_font(m_batt_percent, &lv_font_montserrat_14, LV_PART_MAIN);

    lv_obj_t* data_area = lv_obj_create(m_root);
    lv_obj_set_size(data_area, 200, 168);
    lv_obj_set_style_border_width(data_area, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(data_area, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_pad_all(data_area, 0, LV_PART_MAIN);
    lv_obj_set_flex_flow(data_area, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(data_area, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    auto createDataRow = [data_area](lv_obj_t*& label_out,
                                      const char* symbol, const char* init_text) {
        lv_obj_t* row = lv_obj_create(data_area);
        lv_obj_set_size(row, 200, 48);
        lv_obj_set_style_border_width(row, 0, LV_PART_MAIN);
        lv_obj_set_style_bg_color(row, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_pad_all(row, 0, LV_PART_MAIN);
        lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        lv_obj_t* sym = lv_label_create(row);
        lv_label_set_text(sym, symbol);
        lv_obj_set_style_text_color(sym, lv_color_black(), LV_PART_MAIN);
        lv_obj_set_style_text_font(sym, &lv_font_montserrat_18, LV_PART_MAIN);

        label_out = lv_label_create(row);
        lv_label_set_text(label_out, init_text);
        lv_obj_set_style_text_color(label_out, lv_color_black(), LV_PART_MAIN);
        lv_obj_set_style_text_font(label_out, &lv_font_montserrat_18, LV_PART_MAIN);
    };

    createDataRow(m_datetime_label, LV_SYMBOL_GPS, "--");
    createDataRow(m_temp_label, LV_SYMBOL_CHARGE, "Temp: --.- C");
    createDataRow(m_humi_label, LV_SYMBOL_TINT, "Humi: -- %RH");

    m_update_timer = lv_timer_create(updateTimerCallback, 3000, this);
}

void DashboardPage::update() {
    dashboardModel.update();
    const DashboardModel::Status& s = dashboardModel.getStatus();

    char buf[32];

    snprintf(buf, sizeof(buf), "Temp: %.1f C", s.temp);
    lv_label_set_text(m_temp_label, buf);

    snprintf(buf, sizeof(buf), "Humi: %.0f %%RH", s.humidity);
    lv_label_set_text(m_humi_label, buf);

    if (s.rtc_time.length() > 0) {
        snprintf(buf, sizeof(buf), "%s %s",
                 s.rtc_time.substr(5, 5).c_str(),
                 s.rtc_time.substr(11, 8).c_str());
        lv_label_set_text(m_datetime_label, buf);
    }

    if (s.wifi_connected) {
        lv_label_set_text(m_wifi_icon, LV_SYMBOL_WIFI);
    } else {
        lv_label_set_text(m_wifi_icon, "");
    }

    if (s.sd_mounted) {
        lv_label_set_text(m_sd_icon, LV_SYMBOL_SD_CARD);
    } else {
        lv_label_set_text(m_sd_icon, "");
    }

    lv_label_set_text(m_batt_icon, getBatteryIcon(s.battery_percent));
    snprintf(buf, sizeof(buf), "%d%%", s.battery_percent);
    lv_label_set_text(m_batt_percent, buf);
}

void DashboardPage::updateTimerCallback(lv_timer_t* timer) {
    DashboardPage* page = static_cast<DashboardPage*>(lv_timer_get_user_data(timer));
    if (page) {
        page->update();
    }
}
