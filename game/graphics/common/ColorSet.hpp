#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Все поддерживаемые цветовые коды и стили сообщений (текста).
//   Для удобства чтения и написания форматированных сообщений.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

// Стили
#define RESET         std::wstring(L"&r")
#define BOLD          std::wstring(L"&l")
#define ITALIC        std::wstring(L"&o")
#define UNDERLINED    std::wstring(L"&n")
#define STRIKETHROUGH std::wstring(L"&m")
// Цвета
#define WHITE         std::wstring(L"&f")
#define GRAY          std::wstring(L"&7")
#define BLACK         std::wstring(L"&0")
#define YELLOW        std::wstring(L"&e")
#define GOLD          std::wstring(L"&6")
#define GREEN         std::wstring(L"&a")
#define RED           std::wstring(L"&c")
#define AQUA          std::wstring(L"&b")
#define BLUE          std::wstring(L"&9")
#define MAGENTA       std::wstring(L"&d")


#include <SFML/Graphics/Color.hpp>

namespace awd::game {

    class ColorSet {
    public:
        // https://www.rapidtables.com/web/color/RGB_Color.html
        static const sf::Color
                // Логотип
                LOGO_FILL,
                LOGO_OUTLINE,
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

