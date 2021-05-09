// Общие параметры выделения
#define MAX_EFFECTIVE_HOVER_TICKS 5
// Контур кнопок
#define OUTLINE_WIDTH 2.0f
#define BASE_OUTLINE_ALPHA 135
#define BONUS_OUTLINE_ALPHA_PER_HOVER_TICK 20
// Закрашивание кнопок
#define BASE_FILL_ALPHA 150
#define BONUS_FILL_ALPHA_PER_HOVER_TICK 8
// Текст кнопок
#define BASE_TEXT_ALPHA 180
#define BONUS_TEXT_ALPHA_PER_HOVER_TICK 8
#define TEXT_BOTTOM_MARGIN_HEIGHT_PART 0.1575f


#include <iostream>
#include "TextButton.hpp"
#include "../../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    TextButton::TextButton(id_type id,
                           float renderScale,
                           const std::shared_ptr<sf::RenderWindow>& window,
                           const std::wstring& text,
                           float x, float y,
                           float width, float height,
                           uint32_t fontSize,
                           const std::shared_ptr<ButtonListener>& listener)
                           : Button(id, renderScale, window, listener) {
        this->x      = x;
        this->y      = y;
        this->width  = width;
        this->height = height;

        // Контур кнопки
        float outlineWidth = OUTLINE_WIDTH * renderScale + 1.0f; // min 1 px

        outline = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, height));
        outline->setFillColor(sf::Color::Transparent);
        outline->setOutlineColor(sf::Color::Transparent);
        outline->setOutlineThickness(outlineWidth);
        outline->setPosition(x, y);

        // Закрашивание кнопки
        fill = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, height));

        fill->setFillColor(sf::Color::Transparent);
        fill->setPosition(x, y);

        // Текст на кнопке
        uint32_t textAlpha  = BASE_TEXT_ALPHA + BONUS_TEXT_ALPHA_PER_HOVER_TICK * hoverTicks;

        buttonText = std::make_unique<sf::Text>();
        buttonText->setFont(*Game::instance().getFontManager()->getRegularFont());
        buttonText->setString(text);
        buttonText->setCharacterSize(fontSize);
        buttonText->setFillColor(sf::Color(255, 255, 255, textAlpha));

        sf::FloatRect textBounds = buttonText->getGlobalBounds();
        float textX = x + width  / 2.0f - textBounds.width  / 2.0f;
        float textY = y + height / 2.0f - textBounds.height / 2.0f - height * TEXT_BOTTOM_MARGIN_HEIGHT_PART;

        buttonText->setPosition(textX, textY);
    }

    void TextButton::update() {
        Button::update();

        if (enabled && isMouseOver()) {
            if (hoverTicks < MAX_EFFECTIVE_HOVER_TICKS)
                hoverTicks++;
        } else if (hoverTicks > 0)
            hoverTicks--;

        // Контур кнопки
        uint32_t outlineAlpha = BASE_OUTLINE_ALPHA + BONUS_OUTLINE_ALPHA_PER_HOVER_TICK * hoverTicks;
        outline->setOutlineColor(sf::Color(255, 255, 255, outlineAlpha));

        // Закрашивание кнопки
        uint32_t fillAlpha = BASE_FILL_ALPHA + BONUS_FILL_ALPHA_PER_HOVER_TICK * hoverTicks;
        fill->setFillColor(sf::Color(0, 0, 0, fillAlpha));

        // Текст на кнопке
        uint32_t textAlpha  = BASE_TEXT_ALPHA + BONUS_TEXT_ALPHA_PER_HOVER_TICK * hoverTicks;
        buttonText->setFillColor(sf::Color(255, 255, 255, textAlpha));
    }

    void TextButton::draw() {
        Button::draw();

        window->draw(*outline);
        window->draw(*fill);
        window->draw(*buttonText);
    }

}
