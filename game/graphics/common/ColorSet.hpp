#pragma once


#include <SFML/Graphics/Color.hpp>

namespace awd::game {

    class ColorSet {
    public:
        static const sf::Color
                // Логотип
                LOGO_FILL,
                LOGO_OUTLINE,
                // Вода
                HIGHEST_LEVEL_WATER,
                MIDDLE_LEVEL_WATER,
                LOWEST_LEVEL_WATER,
                WATER_FLICKERS,
                WATER_BUBBLES_FILL,
                WATER_BUBBLES_OUTLINE;
    };

}

