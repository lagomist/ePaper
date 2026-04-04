#pragma once

#include "lvgl.h"
#include "dashboard_model.h"

class DashboardPage {
public:
    DashboardPage();
    ~DashboardPage();

    void create(lv_obj_t* parent);
    void update();

private:
    static void updateTimerCallback(lv_timer_t* timer);

    lv_obj_t* m_root = nullptr;
    lv_obj_t* m_date_label = nullptr;
    lv_obj_t* m_temp_label = nullptr;
    lv_obj_t* m_humi_label = nullptr;
    lv_obj_t* m_batt_label = nullptr;
    lv_obj_t* m_sd_label = nullptr;
    lv_obj_t* m_wifi_label = nullptr;

    lv_timer_t* m_update_timer = nullptr;
};

extern DashboardPage dashboardPage;
