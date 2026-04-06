#pragma once


#include "os_wrapper.h"
#include "board_audio.h"
#include <string>

class MusicPlayer {
public:
    void init(BoardAudio *dev);
    void play(std::string_view path);
private:
    static void music_audio_task(void *arg);

    bool suspend = false;
    bool is_playing = false;
    BoardAudio *device;
    Wrapper::OS::Task thread;
    std::string path;
};

extern MusicPlayer musicPlayer;

