#include <iostream>
#include "RenderUtils.hpp"
#include "../graphics/common/ColorSet.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void RenderUtils::applyFormatting(sfe::RichText& msg, wchar_t code) {
        switch (code) {
            // Сброс стиля и цвета
            case L'r':
            case L'R':
                msg << sf::Text::Regular << ColorSet::MESSAGE_TEXT_WHITE;
                break;

            // Стили
            case L'l':
            case L'L':
                msg << sf::Text::Bold;
                break;

            case L'o':
            case L'O':
                msg << sf::Text::Italic;
                break;

            case L'n':
            case L'N':
                msg << sf::Text::Underlined;
                break;

            case L'm':
            case L'M':
                msg << sf::Text::StrikeThrough;
                break;

            // Цвета
            case L'f':
            case L'F':
                msg << ColorSet::MESSAGE_TEXT_WHITE;
                break;

            case L'7':
                msg << ColorSet::MESSAGE_TEXT_GRAY;
                break;

            case L'0':
                msg << ColorSet::MESSAGE_TEXT_BLACK;
                break;

            case L'e':
            case L'E':
                msg << ColorSet::MESSAGE_TEXT_YELLOW;
                break;

            case L'6':
                msg << ColorSet::MESSAGE_TEXT_GOLD;
                break;

            case L'a':
            case L'A':
                msg << ColorSet::MESSAGE_TEXT_GREEN;
                break;

            case L'c':
            case L'C':
                msg << ColorSet::MESSAGE_TEXT_RED;
                break;

            case L'b':
            case L'B':
                msg << ColorSet::MESSAGE_TEXT_AQUA;
                break;

            case L'9':
                msg << ColorSet::MESSAGE_TEXT_BLUE;
                break;

            case L'd':
            case L'D':
                msg << ColorSet::MESSAGE_TEXT_MAGENTA;
                break;

            // Неизвестный код - пропускаем
            default:
                break;
        }
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void RenderUtils::enrichText(sfe::RichText& msg, const std::wstring& message) {
        // Форматирование.
        std::wstring::size_type offset = 0; // отступ слева от начала текста при выводе очередной части сообщения
        applyFormatting(msg, L'r'); // сброс цвета на белый при каждом выводе

        for (std::wstring::size_type i = 0; i < message.length() - 1; i++) {
            wchar_t code = message[i + 1];

            if (message[i] == '&' && code != '&') { // нашли цветовой код
                // Выводим текст слева от нового цветового кода.
                std::wstring prev = message.substr(offset, i - offset);
                msg << prev;

                applyFormatting(msg, code); // устанавливаем цвет для вывода текста справа от нового цветового кода
                offset = i + 2; // пропускаем сам цветовой код при следующем выводе
                i++; // перескакиваем через следующий символ (цветовой код после '&')
            }
        }

        std::wstring remainder = message.substr(offset, message.length());  // оставшийся текст (если есть)
        msg << remainder; // ^
        applyFormatting(msg, L'r'); // сброс цвета на белый после каждого вывода
    }

    void RenderUtils::verticalGradientSquare(const std::shared_ptr<sf::RenderWindow>& window,
                                             float startX, float startY, float endX, float endY,
                                             const sf::Color& topColor, const sf::Color& bottomColor) {
        sf::Vertex gradient[] = { // координаты точек прямоугольника в порядке по часовой стрелке
                sf::Vertex(sf::Vector2f(startX, startY), topColor),
                sf::Vertex(sf::Vector2f(endX, startY), topColor),
                sf::Vertex(sf::Vector2f(endX, endY), bottomColor),
                sf::Vertex(sf::Vector2f(startX, endY), bottomColor),
        };

        window->draw(gradient, 4, sf::Quads);
    }

    void RenderUtils::turnLeft(const std::shared_ptr<sf::Sprite>& sprite) {
        auto size  = sprite->getTexture()->getSize();
        int  w     = size.x; // NOLINT(cppcoreguidelines-narrowing-conversions)
        int  h     = size.y; // NOLINT(cppcoreguidelines-narrowing-conversions)
        sprite->setTextureRect(sf::IntRect(w, 0, -w, h));
    }

    void RenderUtils::turnRight(const std::shared_ptr<sf::Sprite>& sprite) {
        auto size  = sprite->getTexture()->getSize();
        int  w     = size.x; // NOLINT(cppcoreguidelines-narrowing-conversions)
        int  h     = size.y; // NOLINT(cppcoreguidelines-narrowing-conversions)
        sprite->setTextureRect(sf::IntRect(0, 0, w, h));
    }

    void RenderUtils::turnTop(const std::shared_ptr<sf::Sprite>& sprite) {
        turnRight(sprite); // "top-right" - стандартная позиция (по умолчанию) --> преобразование одно и то же
    }

    void RenderUtils::turnBottom(const std::shared_ptr<sf::Sprite>& sprite) {
        auto size  = sprite->getTexture()->getSize();
        int  w     = size.x; // NOLINT(cppcoreguidelines-narrowing-conversions)
        int  h     = size.y; // NOLINT(cppcoreguidelines-narrowing-conversions)
        sprite->setTextureRect(sf::IntRect(0, h, w, -h));
    }

}
