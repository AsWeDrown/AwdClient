#pragma once


#include <SFML/Graphics/Color.hpp>

namespace awd::game {

    class ColorSet {
    public:
        // https://www.rapidtables.com/web/color/RGB_Color.html
        static const sf::Color
                // Логотип
                LOGO_FILL,
                LOGO_OUTLINE,
                // Общие элементы GUI
                GUI_BUTTONS_SEPARATOR_LINE,
                // Вода
                HIGHEST_LEVEL_WATER,
                MIDDLE_LEVEL_WATER,
                LOWEST_LEVEL_WATER,
                WATER_FLICKERS,
                WATER_BUBBLES,
                // Текст сообщений
                MESSAGE_TEXT_WHITE,
                MESSAGE_TEXT_GRAY,
                MESSAGE_TEXT_BLACK,
                MESSAGE_TEXT_YELLOW,
                MESSAGE_TEXT_GOLD,
                MESSAGE_TEXT_GREEN,
                MESSAGE_TEXT_RED,
                MESSAGE_TEXT_AQUA,
                MESSAGE_TEXT_BLUE,
                MESSAGE_TEXT_MAGENTA;
    };

}

