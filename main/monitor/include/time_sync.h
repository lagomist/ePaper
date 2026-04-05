#pragma once

#include "os_wrapper.h"
#include "time_wrapper.h"
#include "bsp.h"
#include <cstdint>

class TimeSync {
public:
    static constexpr uint32_t SYNC_INTERVAL_MS = 5 * 60 * 1000;
    static constexpr uint32_t MIN_EPOCH_THRESHOLD = 1704067200;

    void init();
    void start();
    void stop();

    bool isSynced() const { return m_synced; }

private:
    static void taskEntry(void* arg);
    void taskLoop();
    void syncToRtc();

    Wrapper::OS::Task m_task;
    bool m_running = false;
    bool m_synced = false;
};

extern TimeSync timeSync;
