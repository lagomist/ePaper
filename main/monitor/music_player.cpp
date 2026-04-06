#include "music_player.h"
#include "fs_wrapper.h"
#include "esp_log.h"
#include <array>

constexpr static const char* TAG = "MusicPlayer";

MusicPlayer musicPlayer;

void MusicPlayer::music_audio_task(void* arg) {
    MusicPlayer* self = static_cast<MusicPlayer*>(arg);
    self->device->setVolume(80);
    std::array<uint8_t, 512> buf;

    while (true) {
        self->thread.notifyWait();

        int file_size = Wrapper::FileSystem::get_file_size(self->path);
        if (file_size <= 0) {
            ESP_LOGE(TAG, "File error: %s", self->path.data());
            continue;
        }

        self->is_playing = true;
        size_t bytes_read = 0;
        while (bytes_read < static_cast<size_t>(file_size) && self->is_playing) {
            size_t chunk = std::min(static_cast<size_t>(file_size) - bytes_read, buf.size());
            int r = Wrapper::FileSystem::read_at(self->path, buf.data(), bytes_read, chunk);
            if (r <= 0) break;
            self->device->playbackWrite(buf.data(), r);
            bytes_read += r;
        }
        self->is_playing = false;
        ESP_LOGI(TAG, "Playback done: %d bytes", bytes_read);
    }
}

void MusicPlayer::play(std::string_view path) {
    if (is_playing) {
        is_playing = false;
        Wrapper::OS::delay(300);
    }
    this->path = path;
    thread.notify(1);
}

void MusicPlayer::init(BoardAudio* dev) {
    device = dev;
    thread.create(music_audio_task, this, "music_audio", 10 * 1024, 10);
}
