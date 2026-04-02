#include "board_codec.h"
#include "i2c_wrapper.h"
#include "driver/i2s_std.h"
#include "driver/i2s_tdm.h"
#include "driver/i2s_pdm.h"
#include "soc/soc_caps.h"
#include "esp_log.h"


static constexpr const char TAG[] = "codec";

int CodecES8311::i2c_bus_inited(I2cConfig i2c_cfg) {
    if (Wrapper::I2C::isInited(i2c_cfg.port)) {
        return 0;
    }
    Wrapper::I2C::init(i2c_cfg.port, i2c_cfg.sda, i2c_cfg.scl, true);
    return 0;
}


int CodecES8311::i2s_bus_init(I2sConfig i2s_cfg, esp_codec_dev_type_t dev_type, CodecCfg codec_cfg) {
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    chan_cfg.auto_clear = true;
    i2s_std_config_t std_cfg = {};
    std_cfg.clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(16000);
    std_cfg.slot_cfg.data_bit_width = I2S_DATA_BIT_WIDTH_32BIT;
    std_cfg.slot_cfg.slot_bit_width = I2S_SLOT_BIT_WIDTH_AUTO;
    std_cfg.slot_cfg.slot_mode = I2S_SLOT_MODE_STEREO;
    std_cfg.slot_cfg.slot_mask = I2S_STD_SLOT_BOTH;
    std_cfg.slot_cfg.ws_width = 32;
    std_cfg.slot_cfg.ws_pol = false;
    std_cfg.slot_cfg.bit_shift = false;
    std_cfg.slot_cfg.left_align = true;
    std_cfg.slot_cfg.big_endian = false;
    std_cfg.slot_cfg.bit_order_lsb = false;

    std_cfg.gpio_cfg.mclk = (gpio_num_t )i2s_cfg.mclk;
    std_cfg.gpio_cfg.bclk = (gpio_num_t )i2s_cfg.bclk;
    std_cfg.gpio_cfg.ws = (gpio_num_t )i2s_cfg.ws;
    std_cfg.gpio_cfg.dout = (gpio_num_t )i2s_cfg.dout;
    std_cfg.gpio_cfg.din = (gpio_num_t )i2s_cfg.din;

#ifdef SOC_I2S_SUPPORTS_TDM
    i2s_tdm_slot_mask_t slot_mask = (i2s_tdm_slot_mask_t )(I2S_TDM_SLOT0 | I2S_TDM_SLOT1 | I2S_TDM_SLOT2 | I2S_TDM_SLOT3);
    i2s_tdm_config_t tdm_cfg = {};
    tdm_cfg.slot_cfg.data_bit_width = I2S_DATA_BIT_WIDTH_32BIT;
    tdm_cfg.slot_cfg.slot_bit_width = I2S_SLOT_BIT_WIDTH_AUTO;
    tdm_cfg.slot_cfg.slot_mode = I2S_SLOT_MODE_STEREO;
    tdm_cfg.slot_cfg.slot_mask = slot_mask;

    tdm_cfg.clk_cfg = I2S_TDM_CLK_DEFAULT_CONFIG(16000),
    tdm_cfg.gpio_cfg.mclk = (gpio_num_t )i2s_cfg.mclk,
    tdm_cfg.gpio_cfg.bclk = (gpio_num_t )i2s_cfg.bclk,
    tdm_cfg.gpio_cfg.ws = (gpio_num_t )i2s_cfg.ws,
    tdm_cfg.gpio_cfg.dout = (gpio_num_t )i2s_cfg.dout,
    tdm_cfg.gpio_cfg.din = (gpio_num_t )i2s_cfg.din,
    tdm_cfg.slot_cfg.total_slot = 4;
#endif
    chan_cfg.id = I2S_NUM_AUTO; // Use auto ID
    int ret = i2s_new_channel(&chan_cfg, &tx_handle, &rx_handle);
    if (tx_handle) {
        if (codec_cfg.mode == I2sMode::CODEC_STD) {
            ret = i2s_channel_init_std_mode(tx_handle, &std_cfg);
            ESP_LOGI(TAG, "output init std ret %d", ret);
        }
#ifdef SOC_I2S_SUPPORTS_TDM
        else if (codec_cfg.mode == I2sMode::CODEC_TDM) {
            ret = i2s_channel_init_tdm_mode(tx_handle, &tdm_cfg);
            ESP_LOGI(TAG, "output init tdm ret %d", ret);
        }
#endif
#ifdef SOC_I2S_SUPPORTS_PDM_TX
        else if (codec_cfg.mode == I2sMode::CODEC_PDM) {
            i2s_pdm_tx_config_t pdm_cfg = {};
            pdm_cfg.clk_cfg = I2S_PDM_TX_CLK_DEFAULT_CONFIG(16000);
            pdm_cfg.slot_cfg.data_bit_width = I2S_DATA_BIT_WIDTH_16BIT;
            pdm_cfg.slot_cfg.slot_bit_width = I2S_SLOT_BIT_WIDTH_AUTO;
            pdm_cfg.slot_cfg.slot_mode = I2S_SLOT_MODE_STEREO;
            pdm_cfg.slot_cfg.data_fmt = I2S_PDM_DATA_FMT_PCM;

            pdm_cfg.gpio_cfg.dout = (gpio_num_t )i2s_cfg.dout;
            pdm_cfg.gpio_cfg.clk = (gpio_num_t )i2s_cfg.bclk;
            pdm_cfg.gpio_cfg.invert_flags.clk_inv = false;
            ret = i2s_channel_init_pdm_tx_mode(tx_handle, &pdm_cfg);
            ESP_LOGI(TAG, "output init pdm ret %d", ret);
        }
#endif
    }
    if (rx_handle) {
        if (codec_cfg.mode == I2sMode::CODEC_STD) {
            ret = i2s_channel_init_std_mode(rx_handle, &std_cfg);
            ESP_LOGI(TAG, "Input init std ret %d", ret);
        }
#ifdef SOC_I2S_SUPPORTS_TDM
        else if (codec_cfg.mode == I2sMode::CODEC_TDM) {
            ret = i2s_channel_init_tdm_mode(rx_handle, &tdm_cfg);
            ESP_LOGI(TAG, "Input init tdm ret %d", ret);
        }
#endif
#ifdef SOC_I2S_SUPPORTS_PDM_RX
        else if (codec_cfg.mode == I2sMode::CODEC_PDM) {
            i2s_pdm_rx_config_t pdm_cfg = {};
            pdm_cfg.clk_cfg = I2S_PDM_RX_CLK_DEFAULT_CONFIG(16000);
            pdm_cfg.slot_cfg.data_bit_width = I2S_DATA_BIT_WIDTH_16BIT;
            pdm_cfg.slot_cfg.slot_bit_width = I2S_SLOT_BIT_WIDTH_AUTO;
            pdm_cfg.slot_cfg.slot_mode = I2S_SLOT_MODE_STEREO;
            pdm_cfg.slot_cfg.slot_mask = I2S_PDM_SLOT_BOTH;
            pdm_cfg.slot_cfg.data_fmt = I2S_PDM_DATA_FMT_PCM;

            pdm_cfg.gpio_cfg.din = (gpio_num_t )i2s_cfg.din;
            pdm_cfg.gpio_cfg.clk = (gpio_num_t )i2s_cfg.bclk;
            pdm_cfg.gpio_cfg.invert_flags.clk_inv = false;
            ret = i2s_channel_init_pdm_rx_mode(rx_handle, &pdm_cfg);
            ESP_LOGI(TAG, "Input init pdm ret %d", ret);
        }
#endif
    }
    // Enable I2S here for maybe some codec need I2S clock to set register correctly
    if (tx_handle) {
        i2s_channel_enable(tx_handle);
    }
    if (rx_handle) {
        i2s_channel_enable(rx_handle);
    }
    ESP_LOGI(TAG, "Init i2s %d ok", i2s_cfg.port);
    return ret;
}

int CodecES8311::i2s_deinit() {
    if (tx_handle) {
        i2s_channel_disable(tx_handle);
    }
    if (rx_handle) {
        i2s_channel_disable(rx_handle);
    }
    if (tx_handle) {
        i2s_del_channel(tx_handle);
    }
    if (rx_handle) {
        i2s_del_channel(rx_handle);
    }
    return 0;
}

int CodecES8311::es8311_codec_init(CodecCfg codec_cfg) {
    es8311_codec_cfg_t es8311_cfg = {};
    es8311_cfg.codec_mode = ESP_CODEC_DEV_WORK_MODE_BOTH;
    es8311_cfg.ctrl_if = codec_res.out_ctrl_if;
    es8311_cfg.gpio_if = codec_res.gpio_if;
    es8311_cfg.pa_pin = codec_cfg.pa_pin;
    es8311_cfg.use_mclk = codec_cfg.use_mclk;
    es8311_cfg.hw_gain.pa_gain = codec_cfg.pa_gain;

    codec_res.out_codec_if = es8311_codec_new(&es8311_cfg);

    esp_codec_dev_cfg_t dev_cfg = {
        .dev_type = ESP_CODEC_DEV_TYPE_IN_OUT,
        .codec_if = codec_res.out_codec_if,
        .data_if = codec_res.data_if,
    };
    codec_res.play_dev = esp_codec_dev_new(&dev_cfg);
    codec_res.record_dev = codec_res.play_dev;
    
    // Set default volume and gain for play and record
    if (codec_res.play_dev) {
        esp_codec_dev_set_out_vol(codec_res.play_dev, 60.0);
    }
    if (codec_res.record_dev) {
        esp_codec_dev_set_in_gain(codec_res.record_dev, 30.0);
    }
    if ((codec_res.play_dev != NULL) || (codec_res.record_dev != NULL)) {
        codec_res.inited = true;
        return 0;
    }
    return -1;
}

int CodecES8311::init(I2cConfig i2c_cfg, I2sConfig i2s_cfg, CodecCfg codec_cfg) {
    if (isInited()) {
        ESP_LOGI(TAG, "Already initialized");
        return 0;
    }
    // Init i2c and i2s
    if (i2c_bus_inited(i2c_cfg) < 0) {
        ESP_LOGE(TAG, "Fail to int i2c: %d", i2c_cfg.port);
        return -1;
    }
    ESP_LOGI(TAG, "Success to int i2c: %d", i2c_cfg.port);
    if (i2s_bus_init(i2s_cfg, ESP_CODEC_DEV_TYPE_IN_OUT, codec_cfg)) {
        ESP_LOGE(TAG, "Fail to init i2s: %d", i2s_cfg.port);
        return -1;
    }
    ESP_LOGI(TAG, "Success to init i2s: %d", i2s_cfg.port);

    // Create gpio interface
    codec_res.gpio_if = audio_codec_new_gpio();

    audio_codec_i2s_cfg_t i2s_out_cfg = {
        .port = i2s_cfg.port,
        .rx_handle = this->rx_handle,
        .tx_handle = this->tx_handle,
    };
    ESP_LOGI(TAG, "Get out handle %p port %d", tx_handle, i2s_cfg.port);
    codec_res.data_if = audio_codec_new_i2s_data(&i2s_out_cfg);

    audio_codec_i2c_cfg_t codec_i2c_cfg = {};
    codec_i2c_cfg.port = i2c_cfg.port;
    codec_i2c_cfg.addr = i2c_cfg.i2c_addr ? i2c_cfg.i2c_addr : ES8311_CODEC_DEFAULT_ADDR;
    codec_i2c_cfg.bus_handle = Wrapper::I2C::getBusHandle(i2c_cfg.port);

    codec_res.out_ctrl_if = audio_codec_new_i2c_ctrl(&codec_i2c_cfg);

    return es8311_codec_init(codec_cfg);
}

esp_codec_dev_handle_t CodecES8311::get_playback_handle(void) {
    return codec_res.play_dev;
}

esp_codec_dev_handle_t CodecES8311::get_record_handle(void) {
    return codec_res.record_dev;
}

void CodecES8311::deinit(void) {
    if (codec_res.play_dev) {
        esp_codec_dev_delete(codec_res.play_dev);
        if (codec_res.record_dev == codec_res.play_dev) {
            codec_res.record_dev = NULL;
        }
        codec_res.play_dev = NULL;
    }
    if (codec_res.record_dev) {
        esp_codec_dev_delete(codec_res.record_dev);
        codec_res.record_dev = NULL;
    }
    // Delete codec interface
    if (codec_res.in_codec_if) {
        audio_codec_delete_codec_if(codec_res.in_codec_if);
        codec_res.in_codec_if = NULL;
    }
    if (codec_res.out_codec_if) {
        audio_codec_delete_codec_if(codec_res.out_codec_if);
        codec_res.out_codec_if = NULL;
    }
    // Delete codec control interface
    if (codec_res.in_ctrl_if) {
        audio_codec_delete_ctrl_if(codec_res.in_ctrl_if);
        codec_res.in_ctrl_if = NULL;
    }
    if (codec_res.out_ctrl_if) {
        audio_codec_delete_ctrl_if(codec_res.out_ctrl_if);
        codec_res.out_ctrl_if = NULL;
    }
    if (codec_res.data_if) {
        audio_codec_delete_data_if(codec_res.data_if);
        codec_res.data_if = NULL;
    }
    if (codec_res.data_in_if) {
        audio_codec_delete_data_if(codec_res.data_in_if);
        codec_res.data_in_if = NULL;
    }
    if (codec_res.gpio_if) {
        audio_codec_delete_gpio_if(codec_res.gpio_if);
        codec_res.gpio_if = NULL;
    }
    i2s_deinit();
    codec_res.inited = false;
}

