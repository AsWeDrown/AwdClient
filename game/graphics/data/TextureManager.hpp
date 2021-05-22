#pragma once


#include <memory>
#include <SFML/Graphics.hpp>


namespace awd::game {

    class TextureManager {
    private:
        bool loadCharAnim(uint32_t character, uint32_t animation);
        bool loadCharacters();
        bool loadWorldTileMap();

    public:
        // Здесь ключ       - номер (ID) персонажа,
        //       значение   - список текстур соответствующего персонажа (разные его анимации),
        //                    где ключ     - номер (ID) анимации из класса Entities::EntityPlayer,
        //                        значение - текстура соответствующей анимации соответствующего персонажа.
        //
        // Пример: characters[1][2] = текстура анимации #2 персонажа #1.
        std::map<uint32_t, std::map<uint32_t, std::shared_ptr<sf::Texture>>> characters;

        std::shared_ptr<sf::Texture> worldTileMap;

        bool loadTextures();
    };

}

