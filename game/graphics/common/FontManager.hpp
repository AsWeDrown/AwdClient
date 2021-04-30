#pragma once


#include <memory>
#include <SFML/Graphics.hpp>

namespace awd::game {

    class FontManager {
    private:
        std::shared_ptr<sf::Font> regularFont    = nullptr;
        std::shared_ptr<sf::Font> decorativeFont = nullptr;

    public:
        bool loadFonts();

        std::shared_ptr<sf::Font> getRegularFont   () const;
        std::shared_ptr<sf::Font> getDecorativeFont() const;
    };

}
