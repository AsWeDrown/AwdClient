#pragma once


#include <memory>
#include <SFML/Graphics.hpp>


namespace awd::game {

    class TextureManager {
    private:
        bool loadCharacters();

    public:
        std::vector<std::shared_ptr<sf::Texture>> characters;

        bool loadTextures();
    };

}

