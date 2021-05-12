#pragma once


#include <memory>
#include <SFML/Graphics.hpp>


namespace awd::game {

    class TextureManager {
    private:
        bool loadCharacters();
        bool loadWorldTileMap();

    public:
        std::vector<std::shared_ptr<sf::Texture>> characters;
                    std::shared_ptr<sf::Texture>  worldTileMap;

        bool loadTextures();
    };

}

