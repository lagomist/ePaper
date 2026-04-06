#include "board_audio.h"
#include "esp_heap_caps.h"

void BoardAudio::setVolume(uint8_t vol) {
	esp_codec_dev_handle_t playback = get_playback_handle();
  	esp_codec_dev_set_out_vol(playback, vol);
}

void BoardAudio::playerInit(void) {
	esp_codec_dev_handle_t playback = get_playback_handle();
	esp_codec_dev_handle_t record = get_record_handle();
	esp_codec_dev_set_out_vol(playback, 100.0); // 设置100声音大小
  	esp_codec_dev_set_in_gain(record, 45.0);   	// 设置录音时的增益
  	esp_codec_dev_sample_info_t fs = {};
	fs.sample_rate = 16000;
	fs.channel = 1;
	fs.bits_per_sample = 16;
  	esp_codec_dev_open(playback, &fs); // 打开播放
  	esp_codec_dev_open(record, &fs);   // 打开录音
}

void BoardAudio::playbackRead(void *data_ptr,uint32_t len) {
	esp_codec_dev_handle_t record = get_record_handle();
	esp_codec_dev_read(record, data_ptr, len);
}

void BoardAudio::playbackWrite(void *data_ptr,uint32_t len) {
	esp_codec_dev_handle_t playback = get_playback_handle();
	esp_codec_dev_write(playback, data_ptr, len);
}
