#include <iostream>
#include "FontManager.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool FontManager::loadFonts() {
        std::cout << "Loading fonts..." << std::endl;

        // REGULAR
        regularFont = std::make_shared<sf::Font>();

        if (!regularFont->loadFromFile("assets/fonts/SubdexBold.ttf")) {
            std::cerr << "Failed to load REGULAR font." << std::endl;
            return false;
        }

        // DECORATIVE
        decorativeFont = std::make_shared<sf::Font>();

        if (!decorativeFont->loadFromFile("assets/fonts/PaperCuts2.ttf")) {
            std::cerr << "Failed to load DECORATIVE font." << std::endl;
            return false;
        }

        return true;
    }

    std::shared_ptr<sf::Font> FontManager::getRegularFont() const {
        return regularFont;
    }

    std::shared_ptr<sf::Font> FontManager::getDecorativeFont() const {
        return decorativeFont;
    }

}
