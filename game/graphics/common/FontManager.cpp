#include <iostream>
#include "FontManager.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool FontManager::loadFonts() {
        std::wcout << L"Loading fonts" << std::endl;

        // REGULAR
        regularFont = std::make_shared<sf::Font>();

        if (!regularFont->loadFromFile("assets/fonts/SubdexBold.ttf")) {
            std::wcerr << L"Failed to load font: regular." << std::endl;
            return false;
        }

        // DECORATIVE
        decorativeFont = std::make_shared<sf::Font>();

        if (!decorativeFont->loadFromFile("assets/fonts/PaperCuts2.ttf")) {
            std::wcerr << L"Failed to load font: decorative." << std::endl;
            return false;
        }

        return true;
    }

}
