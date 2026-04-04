#pragma once

#include "bsp.h"
#include "wifi_wrapper.h"
#include "fs_wrapper.h"
#include <cstdint>
#include <string>

class DashboardModel {
public:
    struct Status {
        float temp = 0.0f;
        float humidity = 0.0f;
        float battery_voltage = 0.0f;
        uint8_t battery_percent = 0;
        std::string rtc_time;
        bool sd_mounted = false;
        bool wifi_connected = false;
        int wifi_rssi = 0;
        bool system_ok = true;
    };

    void init();
    void update();
    const Status& getStatus() const { return m_status; }

private:
    Status m_status;
};

extern DashboardModel dashboardModel;
