#include "board_rtc.h"
#include "i2c_wrapper.h"
#include "esp_log.h"

constexpr static const char TAG[] = "BoardRTC";

BoardRTC::BoardRTC(uint8_t host) {
	i2c_master_bus_handle_t busHandle = (i2c_master_bus_handle_t )Wrapper::I2C::getBusHandle(host);
	bool ret = rtc.begin(busHandle);
	if(!ret){
		ESP_LOGE(TAG, "Initialization failed");
	}
}

BoardRTC::~BoardRTC() {
	
}

void BoardRTC::setRtcTime(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second) {
	rtc.setDateTime(year, month, day, hour, minute, second);
}

BoardRTC::DateTime BoardRTC::getRtcTime() {
	RTC_DateTime datetime = rtc.getDateTime();
  	time.year = datetime.getYear();
  	time.month = datetime.getMonth();
  	time.day = datetime.getDay();
  	time.hour = datetime.getHour();
  	time.minute = datetime.getMinute();
  	time.second = datetime.getSecond();
  	time.week = datetime.getWeek();
	return time;
}
