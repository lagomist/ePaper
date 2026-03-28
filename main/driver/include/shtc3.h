#pragma once

#include "i2c_wrapper.h"
#include <cstdint>


class Shtc3 {
public:
	enum etError {
		NO_ERROR       = 0x00, // no error
		ACK_ERROR      = 0x01, // no acknowledgment error
		CHECKSUM_ERROR = 0x02 // checksum mismatch error
	};

	enum etCommands{
		READ_ID            = 0xEFC8, // command: read ID register
		SOFT_RESET         = 0x805D, // soft reset
		SLEEP              = 0xB098, // sleep
		WAKEUP             = 0x3517, // wakeup
		MEAS_T_RH_POLLING  = 0x7866, // meas. read T first, clock stretching disabled
		MEAS_T_RH_CLOCKSTR = 0x7CA2, // meas. read T first, clock stretching enabled
		MEAS_RH_T_POLLING  = 0x58E0, // meas. read RH first, clock stretching disabled
		MEAS_RH_T_CLOCKSTR = 0x5C24  // meas. read RH first, clock stretching enabled
	};
	struct DataPack {
		float Temp;
		float RH;
	};

	Shtc3(uint8_t i2c_host);
	~Shtc3();

	etError wakeup();
	etError sleep();
	etError softReset();
	uint16_t getId();
	DataPack readTempHumi();

private:
	uint16_t shtc3_id = 0x00;
	constexpr static uint8_t SHTC3_I2C_ADDR = 0x70;
	const uint16_t CRC_POLYNOMIAL = 0x131;
	const uint8_t  SHTC3_PETP_NUM = 4;
	Wrapper::I2C::Device bus;

	etError getShtcId();
	etError checkCrc(uint8_t data[], uint8_t nbrOfBytes,uint8_t checksum);
	etError getTempAndHumiPolling(float *temp, float *humi);
	float calcTemperature(uint16_t rawValue);
	float calcHumidity(uint16_t rawValue);
};
