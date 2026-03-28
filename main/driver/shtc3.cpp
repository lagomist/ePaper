#include "shtc3.h"
#include "os_wrapper.h"
#include "esp_log.h"

constexpr static const char TAG[] = "shtc3";

Shtc3::Shtc3(uint8_t i2c_host) : bus(i2c_host) {
	bus.init(SHTC3_I2C_ADDR, 300000);
	wakeup();
	softReset();
	Wrapper::OS::delay(20);
	getShtcId();
	ESP_LOGI(TAG, "ID:%04x", shtc3_id);
}

Shtc3::~Shtc3() {
}

Shtc3::etError Shtc3::getShtcId() {
	uint8_t senBuf[2] = {(READ_ID>>8),(READ_ID&0xff)};
  	uint8_t readBuf[3] = {0,0,0};
	int err = bus.trans_recv(senBuf, sizeof(senBuf), readBuf, sizeof(readBuf));
  	etError error = (err == 0) ? NO_ERROR : ACK_ERROR;
  	if (error != NO_ERROR)
  	{ESP_LOGE(TAG,"GetId WRITE Failure");return error;}
  	error = checkCrc(readBuf,2,readBuf[2]);
  	if (error != NO_ERROR)
  	{ESP_LOGE(TAG,"GetId CRC Failure");return error;}
  	shtc3_id = ((readBuf[0] << 8) | readBuf[1]);
  	return error;
}

uint16_t Shtc3::getId() {
	return shtc3_id;
}

// wake up the sensor from sleep mode
Shtc3::etError Shtc3::wakeup() {
	uint8_t senBuf[2] = {(WAKEUP>>8),(WAKEUP&0xff)};
	int err = bus.write(senBuf, sizeof(senBuf));
  	etError error = (err == 0) ? NO_ERROR : ACK_ERROR;
	Wrapper::OS::delay(50);
  	if (error != NO_ERROR) {
		ESP_LOGE(TAG,"Wakeup Failure");
	}
  	return error;
}

Shtc3::etError Shtc3::softReset() {
  	uint8_t senBuf[2] = {(SOFT_RESET>>8),(SOFT_RESET&0xff)};
	int err = bus.write(senBuf, sizeof(senBuf));
  	etError error = (err == 0) ? NO_ERROR : ACK_ERROR;
  	if (error != NO_ERROR) {
		ESP_LOGE(TAG,"SoftReset Failure");
	}
  	return error;
}

Shtc3::etError Shtc3::checkCrc(uint8_t data[], uint8_t nbrOfBytes,uint8_t checksum) {
  	uint8_t bit;        // bit mask
  	uint8_t crc = 0xFF; // calculated checksum
  	uint8_t byteCtr;    // byte counter

  	// calculates 8-Bit checksum with given polynomial
  	for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++) {
  	  	crc ^= (data[byteCtr]);
  	  	for(bit = 8; bit > 0; --bit) {
  	  	  	if(crc & 0x80) {
  	  	  	  	crc = (crc << 1) ^ CRC_POLYNOMIAL;
  	  	  	} else {
  	  	  	  	crc = (crc << 1);
  	  	  	}
  	  	}
  	}

  	// verify checksum
  	if(crc != checksum)	{
  	  	return CHECKSUM_ERROR;
  	} else {
  	  	return NO_ERROR;
  	}
}

float Shtc3::calcTemperature(uint16_t rawValue) {
  // calculate temperature [°C]
  // T = -45 + 175 * rawValue / 2^16
  return 175 * (float)rawValue / 65536.0f - 45.0f - SHTC3_PETP_NUM;
}

float Shtc3::calcHumidity(uint16_t rawValue) {
  // calculate relative humidity [%RH]
  // RH = rawValue / 2^16 * 100
  return 100 * (float)rawValue / 65536.0f;
}

Shtc3::etError Shtc3::getTempAndHumiPolling(float *temp, float *humi) {
  	int err = 0;
  	etError  error;           // error code
  	uint16_t rawValueTemp;    // temperature raw value from sensor
  	uint16_t rawValueHumi;    // humidity raw value from sensor
  	uint8_t bytes[6] = {0};;
  	uint8_t senBuf[2] = {(MEAS_T_RH_POLLING>>8),(MEAS_T_RH_POLLING&0xff)};
	err = bus.write(senBuf, sizeof(senBuf));
  	error = (err == 0) ? NO_ERROR : ACK_ERROR;
  	if (error != NO_ERROR)
  	{ESP_LOGE(TAG,"GetTempAndHumi WRITE Failure");return error;}
	
	Wrapper::OS::delay(20);

  	// if no error, read temperature and humidity raw values
	err = bus.read(bytes, 6);
  	error = (err == 0) ? NO_ERROR : ACK_ERROR;
  	if (error != NO_ERROR)
  	{ESP_LOGE(TAG,"GetTempAndHumi READ Failure"); return error;}
  	error = checkCrc(bytes, 2, bytes[2]);
  	if (error != NO_ERROR)
  	{ESP_LOGE(TAG,"GetTempAndHumi TempCRC Failure"); return error;}
  	error = checkCrc(&bytes[3], 2, bytes[5]);
  	if (error != NO_ERROR)
  	{ESP_LOGE(TAG,"GetTempAndHumi humidityCRC Failure"); return error;}
  	// if no error, calculate temperature in °C and humidity in %RH
  	rawValueTemp = (bytes[0] << 8) | bytes[1];
  	rawValueHumi = (bytes[3] << 8) | bytes[4];
  	*temp = calcTemperature(rawValueTemp);
  	*humi = calcHumidity(rawValueHumi);
  	return error;
}

Shtc3::etError Shtc3::sleep() {
  	uint8_t senBuf[2] = {(SLEEP>>8),(SLEEP&0xff)};
	int err = bus.write(senBuf, sizeof(senBuf));
  	etError error = (err == 0) ? NO_ERROR : ACK_ERROR;
  	if (error != NO_ERROR) {
		ESP_LOGE(TAG,"Sleep Failure");
	}
  	return error;
}

Shtc3::DataPack Shtc3::readTempHumi() {
	etError  error;
  	float temperature;
  	float humidity;
  	DataPack shtc3_data;
  	wakeup();
  	error = getTempAndHumiPolling(&temperature, &humidity);
  	if (error != NO_ERROR) {
  	  ESP_LOGE(TAG,"error:%d",error);
  	} else {
  	  shtc3_data.RH = humidity;
  	  shtc3_data.Temp = temperature;
  	}
  	sleep();
  	return shtc3_data;
}
