#include "PlayerCard.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    PlayerCard::PlayerCard(id_type id,
                           float renderScale,
                           const std::shared_ptr<sf::RenderWindow>& window,
                           float x, float y,
                           float width, float height,
                           const std::shared_ptr<ButtonListener>& listener,
                           uint32_t fontSize)
                           : Button(id, renderScale, window, listener) {
        this->x        = x;
        this->y        = y;
        this->width    = width;
        this->height   = height;
        this->fontSize = fontSize;

        // Текстура отображаемого персонажа
        characterSprite = std::make_unique<sf::Sprite>();
        characterSprite->setColor(sf::Color::Red);//todo texture
        characterSprite->setPosition(x, y);
    }

    void PlayerCard::update() {
        Button::update();
    }

    void PlayerCard::draw() {
        Button::draw();

        window->draw(*characterSprite);
    }

    uint32_t PlayerCard::getDisplayedCharacter() const {
        return displayedCharacter;
    }

    void PlayerCard::setDisplayedCharacter(uint32_t character) {
        // todo update sprite texture
        this->displayedCharacter = character;
    }

}
