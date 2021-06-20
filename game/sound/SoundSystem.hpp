#pragma once


#include <memory>
#include <SFML/Audio.hpp>
#include "Sound.hpp"

namespace awd::game {

    class SoundSystem {
    private:
        static constexpr float DIST_VOL_FACTOR = 7.5f;

        uint32_t currentMusicId = 0;

        static float calculateVolume(const Sound& sound);

    public:
        void playSound(uint32_t id); // Global
        void playSound(const Sound& sound);
        void playMusic(uint32_t id);
        void stopMusic();
    };

}

