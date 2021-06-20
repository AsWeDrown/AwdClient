#pragma once


#include <map>
#include <memory>
#include <string>
#include <SFML/Audio.hpp>

namespace awd::game {

    class SoundManager {
    private:
        // И sf::SoundBuffer, и sf::Sound нужно хранить в полях класса. Иначе звука не будет.
        std::map<uint32_t, std::shared_ptr<sf::SoundBuffer>> soundBuffers;
        std::map<uint32_t, std::shared_ptr<sf::Sound      >> sounds;
        std::map<uint32_t, std::shared_ptr<sf::Music      >> music;

        bool loadSound(uint32_t id, const std::string& name);
        bool loadMusic(uint32_t id, const std::string& name, bool loop);

        bool loadAllSounds();
        bool loadAllMusic ();

    public:
        // Возвращать обязательно shared_ptr. Иначе звука не будет.
        std::shared_ptr<sf::Sound> getSoundById(uint32_t id) const;
        std::shared_ptr<sf::Music> getMusicById(uint32_t id) const;

        bool loadSounds();
    };

}

