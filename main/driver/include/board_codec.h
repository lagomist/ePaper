#pragma once

#include "esp_codec_dev.h"
#include "esp_codec_dev_defaults.h"
#include "esp_codec_dev_os.h"
#include "driver/i2s_types.h"
#include <cstdint>


class CodecES8311 {
public:
	enum class I2sMode : uint8_t {
		CODEC_STD = 0, /*!< STD mode */
		CODEC_TDM,     /*!< TDM mode */
		CODEC_PDM,     /*!< PDM mode */
		CODEC_NONE,    /*!< NONE means not support */
	};
	struct I2cConfig {
		int sda;
		int scl;
		int freq_hz;
		uint8_t port;
		uint8_t i2c_addr;
	};
	struct I2sConfig {
		int mclk; /*!< GPIO for MCLK */
		int bclk; /*!< GPIO for BCLK */
		int ws;   /*!< GPIO for Word Selction */
		int dout; /*!< GPIO for digital output */
		int din;  /*!< GPIO for digital input */
		uint8_t port;
	};
	struct CodecCfg {
		int16_t      pa_pin;     /*!< GPIO for PA control */
		float        pa_gain;    /*!< PA gain */
		uint8_t      i2c_addr;   /*!< I2C address */
		bool         use_mclk;   /*!< Whether codec need MCLK clock */
		I2sMode mode;
	};
    int init(I2cConfig i2c_cfg,I2sConfig i2s_cfg, CodecCfg codec_cfg);
	void deinit();
	bool isInited() { return codec_res.inited; }

	esp_codec_dev_handle_t get_playback_handle(void);
	esp_codec_dev_handle_t get_record_handle(void);
private:
	// 禁止拷贝和赋值
    CodecES8311& operator=(const CodecES8311&) = delete;

	struct codecProperty {
		bool                         inited;
		const audio_codec_data_if_t *data_if;
		const audio_codec_data_if_t *data_in_if;
		const audio_codec_gpio_if_t *gpio_if;
		const audio_codec_ctrl_if_t *in_ctrl_if;
		const audio_codec_ctrl_if_t *out_ctrl_if;
		const audio_codec_if_t      *out_codec_if;
		const audio_codec_if_t      *in_codec_if;
		esp_codec_dev_handle_t       play_dev;
		esp_codec_dev_handle_t       record_dev;
	};

	int i2c_bus_inited(I2cConfig i2c_cfg);
	int i2s_bus_init(I2sConfig i2s_cfg, esp_codec_dev_type_t dev_type, CodecCfg codec_cfg);
	int i2s_deinit();
	int es8311_codec_init(CodecCfg codec_cfg);

	bool inited = false;
	codecProperty codec_res;
	i2s_chan_handle_t tx_handle;
    i2s_chan_handle_t rx_handle;
};
