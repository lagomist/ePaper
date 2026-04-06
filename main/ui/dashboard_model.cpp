#include "dashboard_model.h"
#include "esp_log.h"

constexpr static const char* TAG = "DashboardModel";

DashboardModel dashboardModel;

void DashboardModel::init() {
    update();
}

void DashboardModel::update() {
    if (bsp.tempHumi) {
        Shtc3::DataPack data = bsp.tempHumi->readTempHumi();
        m_status.temp = data.Temp;
        m_status.humidity = data.RH;
    }

    if (bsp.vbat) {
        m_status.battery_voltage = bsp.vbat->getVbatVoltage();
        m_status.battery_percent = bsp.vbat->getBatteryLevel();
    }

    if (bsp.rtc) {
        BoardRTC::DateTime dt = bsp.rtc->getRtcTime();
        char buf[32];
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
                 dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
        m_status.rtc_time = std::string(buf);
    }

    m_status.sd_mounted = Wrapper::FileSystem::SdCard::is_mounted();

    m_status.wifi_connected = Wrapper::WiFi::Station::is_connected();
    if (m_status.wifi_connected) {
        m_status.wifi_rssi = Wrapper::WiFi::Station::get_rssi();
    } else {
        m_status.wifi_rssi = 0;
    }

    m_status.system_ok = true;
}
