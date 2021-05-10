#pragma once


#include <memory>
#include <SFML/Graphics.hpp>

namespace awd::game {

    class FontManager {
    public:
        std::shared_ptr<sf::Font> regularFont    = nullptr;
        std::shared_ptr<sf::Font> decorativeFont = nullptr;

        bool loadFonts();
    };

}
