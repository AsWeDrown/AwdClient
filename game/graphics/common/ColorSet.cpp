#include "ColorSet.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   Логотип
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    const sf::Color ColorSet::LOGO_FILL   (255, 255, 255           );
    const sf::Color ColorSet::LOGO_OUTLINE(  0, 224, 186, 115);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   Вода
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    const sf::Color ColorSet::HIGHEST_LEVEL_WATER(0, 204, 166);
    const sf::Color ColorSet::MIDDLE_LEVEL_WATER (0, 141, 122);
    const sf::Color ColorSet::LOWEST_LEVEL_WATER (0, 102,  98);
    const sf::Color ColorSet::WATER_FLICKERS     (0, 224, 186);
    const sf::Color ColorSet::WATER_BUBBLES      (0, 224, 186);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   Текст сообщений
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    const sf::Color ColorSet::MESSAGE_TEXT_WHITE  (250, 250, 250);
    const sf::Color ColorSet::MESSAGE_TEXT_GRAY   (200, 200, 200);
    const sf::Color ColorSet::MESSAGE_TEXT_BLACK  (  0,   0,   0);
    const sf::Color ColorSet::MESSAGE_TEXT_YELLOW (255, 250, 100);
    const sf::Color ColorSet::MESSAGE_TEXT_GOLD   (250, 200, 100);
    const sf::Color ColorSet::MESSAGE_TEXT_GREEN  (100, 255, 100);
    const sf::Color ColorSet::MESSAGE_TEXT_RED    (255,  80,  80);
    const sf::Color ColorSet::MESSAGE_TEXT_AQUA   (110, 250, 255);
    const sf::Color ColorSet::MESSAGE_TEXT_BLUE   (100, 140, 255);
    const sf::Color ColorSet::MESSAGE_TEXT_MAGENTA(230, 100, 250);

}