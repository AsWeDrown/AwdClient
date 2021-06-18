#include <iostream>
#include "FontManager.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool FontManager::loadRegularFont() {
        regularFont = std::make_shared<sf::Font>();

        if (regularFont->loadFromFile("assets/fonts/SubdexBold.ttf"))
            return true;
        else {
            std::wcerr << L"Failed to load font: regular" << std::endl;
            return false;
        }
    }

    bool FontManager::loadDecorativeFont() {
        decorativeFont = std::make_shared<sf::Font>();

        if (decorativeFont->loadFromFile("assets/fonts/PaperCuts2.ttf"))
            return true;
        else {
            std::wcerr << L"Failed to load font: decorative" << std::endl;
            return false;
        }
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool FontManager::loadFonts() {
        std::wcout << L"Loading fonts" << std::endl;

        return loadRegularFont   ()
            && loadDecorativeFont();
    }

}
