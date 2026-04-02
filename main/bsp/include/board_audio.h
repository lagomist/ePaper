#pragma once

#include "board_codec.h"
#include "esp_codec_dev.h"
#include <cstdint>

class BoardAudio : public CodecES8311 {
public:
    void playerInit(void);
    void setVolume(uint8_t vol);

    void playbackRead(void *data_ptr, uint32_t len);

    void playbackWrite(void *data_ptr, uint32_t len);
private:
};



