#include <iostream>
#include "TextureManager.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool TextureManager::loadCharacters() {
        for (int i = 1; i <= 4; i++) { // 4 персонажа
            auto texture = std::make_shared<sf::Texture>();

            if (!texture->loadFromFile(
                    "assets/textures/characters/char_" + std::to_string(i) + ".png")) {
                std::wcerr << L"Failed to load texture for character " << i << std::endl;
                return false;
            }

            characters.push_back(texture);
        }

        return true;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool TextureManager::loadTextures() {
        std::wcout << L"Loading textures" << std::endl;
        return loadCharacters();
    }

}
