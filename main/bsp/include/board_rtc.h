#pragma once

#include "SensorPCF85063.hpp"
#include <cstdint>

class BoardRTC {
public:
    struct DateTime {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint8_t week;
    };
    BoardRTC(uint8_t host);
    ~BoardRTC();
    void setRtcTime(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second);
	DateTime getRtcTime();
private:
    SensorPCF85063 rtc;
	DateTime time;
};



