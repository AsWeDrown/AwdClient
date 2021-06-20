#include <iostream>
#include "SoundManager.hpp"
#include "Sound.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool SoundManager::loadSound(uint32_t id, const std::string& name) {
        auto buffer = std::make_shared<sf::SoundBuffer>();

        if (buffer->loadFromFile("assets/audio/sounds/" + name + ".wav")) {
            auto sound = std::make_shared<sf::Sound>();

            sound->setBuffer(*buffer);
            soundBuffers[id] = buffer;
            sounds[id] = sound;

            return true;
        } else {
            std::wcerr << L"Failed to load sound with ID: " << id << std::endl;
            return false;
        }
    }

    bool SoundManager::loadMusic(uint32_t id, const std::string& name) {
        auto buffer = std::make_shared<sf::Music>();

        if (buffer->openFromFile("assets/audio/music/" + name + ".wav")) {
            buffer->setLoop(true);
            music[id] = buffer;

            return true;
        } else {
            std::wcerr << L"Failed to load music with ID: " << id << std::endl;
            return false;
        }
    }

    bool SoundManager::loadAllSounds() {
        return loadSound(Sound::BUTTON_CLICK, "button_click")
        ;
    }

    bool SoundManager::loadAllMusic() {
        return loadMusic(Sound::MAIN_MENU_THEME, "main_menu_theme")
        ;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    std::shared_ptr<sf::Sound> SoundManager::getSoundById(uint32_t id) const {
        auto cursor = sounds.find(id);

        if (cursor == sounds.cend())
            throw std::runtime_error("no such sound with ID: " + std::to_string(id));

        return cursor->second;
    }

    std::shared_ptr<sf::Music> SoundManager::getMusicById(uint32_t id) const {
        auto cursor = music.find(id);

        if (cursor == music.cend())
            throw std::runtime_error("no such music with ID: " + std::to_string(id));

        return cursor->second;
    }

    bool SoundManager::loadSounds() {
        std::wcout << L"Loading sounds" << std::endl;
        return loadAllSounds() && loadAllMusic();
    }

}
