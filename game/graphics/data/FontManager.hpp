#pragma once


#include <memory>
#include <SFML/Graphics.hpp>

namespace awd::game {

    class FontManager {
    private:
        bool loadRegularFont   ();
        bool loadDecorativeFont();

    public:
        std::shared_ptr<sf::Font> regularFont    = nullptr;
        std::shared_ptr<sf::Font> decorativeFont = nullptr;

        bool loadFonts();
    };

}
