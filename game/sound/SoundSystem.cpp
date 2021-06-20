#include <cmath>
#include "SoundSystem.hpp"
#include "../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    float SoundSystem::calculateVolume(const Sound& sound) {
        if (sound.isGlobal()) // Global
            return 100.0f; // это глобальный звук (не имеет конкретного источника)
        else if (Game::instance().getCurrentState() == GameState::PLAY) {
            auto ownEntity = Game::instance().currentWorld()->
                    getEntityById(Game::instance().getCurrentLobby()->ownEntityId);

            float dx = sound.getSourceX() - ownEntity->getPosX();
            float dy = sound.getSourceY() - ownEntity->getPosY();
            float srcDist = sqrtf(dx * dx + dy * dy);

            return std::min(100.0f, std::max(0.0f, 100.0f - DIST_VOL_FACTOR * srcDist)); // 0..100
        } else
            return 0.0f; // такого не должно быть
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void SoundSystem::playSound(uint32_t id) {
        playSound(Sound(id));
    }

    void SoundSystem::playSound(const Sound& sound) {
        // TODO: более "пространственные" звуки -- https://www.sfml-dev.org/tutorials/2.5/audio-spatialization.php
        auto s = Game::instance().getSounds()->getSoundById(sound.getId());
        s->setVolume(calculateVolume(sound));
        s->play();
    }

    void SoundSystem::playMusic(uint32_t id) {
        stopMusic();
        currentMusicId = id;
        Game::instance().getSounds()->getMusicById(currentMusicId)->play();
    }

    void SoundSystem::stopMusic() {
        if (currentMusicId != 0) {
            Game::instance().getSounds()->getMusicById(currentMusicId)->stop();
            currentMusicId = 0;
        }
    }

}
