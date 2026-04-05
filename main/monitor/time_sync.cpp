#include "time_sync.h"
#include "esp_log.h"
#include "wifi_wrapper.h"
#include <ctime>

constexpr static const char* TAG = "TimeSync";

TimeSync timeSync;

void TimeSync::init() {
    Wrapper::Time::init();
}

void TimeSync::start() {
    if (m_running) {
        return;
    }
    m_running = true;
    m_task.create(taskEntry, this, "time_sync", 4096, 5, -1);
    ESP_LOGI(TAG, "TimeSync task started");
}

void TimeSync::stop() {
    if (!m_running) {
        return;
    }
    m_running = false;
    m_task.del();
    ESP_LOGI(TAG, "TimeSync task stopped");
}

void TimeSync::taskEntry(void* arg) {
    static_cast<TimeSync*>(arg)->taskLoop();
}

void TimeSync::taskLoop() {
    while (m_running) {
        bool wifi_ok = Wrapper::WiFi::Station::is_connected();
        uint32_t now_sec = Wrapper::Time::second();
        m_synced = wifi_ok && (now_sec > MIN_EPOCH_THRESHOLD);

        if (m_synced) {
            syncToRtc();
        }
        for (uint32_t i = 0; i < SYNC_INTERVAL_MS && m_running; i += 1000) {
            Wrapper::OS::delay(1000);
        }
    }
}

void TimeSync::syncToRtc() {
    if (!bsp.rtc) {
        return;
    }

    time_t now = Wrapper::Time::second();
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);

    bsp.rtc->setRtcTime(
        static_cast<uint16_t>(timeinfo.tm_year + 1900),
        static_cast<uint8_t>(timeinfo.tm_mon + 1),
        static_cast<uint8_t>(timeinfo.tm_mday),
        static_cast<uint8_t>(timeinfo.tm_hour),
        static_cast<uint8_t>(timeinfo.tm_min),
        static_cast<uint8_t>(timeinfo.tm_sec)
    );
    ESP_LOGI(TAG, "RTC synced from SNTP: %04d-%02d-%02d %02d:%02d:%02d",
             timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
             timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}
