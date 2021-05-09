// Размер и расположения текста на кнопках
#define BUTTON_TEXT_LEFT_MARGIN 25.0f
#define BUTTON_TEXT_TOP_MARGIN 6.0f
#define FONT_SIZE 45
// Параметры выделенных кнопок
#define VERTICAL_LINE_WIDTH 10.0f
#define MAX_EFFECTIVE_HOVER_TICKS 7
#define LINE_ALPHA_PER_HOVER_TICK 35
#define FILL_ALPHA_PER_HOVER_TICK 15
#define BASE_TEXT_ALPHA 175
#define BONUS_TEXT_ALPHA_PER_HOVER_TICK 7
#define TEXT_OFFSET_PER_HOVER_TICK 1


#include <iostream>
#include "MainMenuButton.hpp"
#include "../../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    
    MainMenuButton::MainMenuButton(id_type id,
                                   float renderScale,
                                   const std::shared_ptr<sf::RenderWindow>& window,
                                   const std::wstring& text,
                                   float x, float y,
                                   float width, float height,
                                   const std::shared_ptr<ButtonListener>& listener)
                                   : Button(id, renderScale, window, listener) {
        this->x      = x;
        this->y      = y;
        this->width  = width;
        this->height = height;

        // Вертикальная черта слева от текста
        float lineWidth = VERTICAL_LINE_WIDTH * renderScale;

        line = std::make_unique<sf::RectangleShape>(sf::Vector2f(lineWidth, height));
        line->setFillColor(sf::Color::Transparent);
        line->setPosition(x, y);

        // Закрашивание кнопки
        fill = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, height));
        fill->setFillColor(sf::Color::Transparent);
        fill->setPosition(x + lineWidth, y);

        // Текст на кнопке
        uint32_t textAlpha  = BASE_TEXT_ALPHA + BONUS_TEXT_ALPHA_PER_HOVER_TICK * hoverTicks;
        uint32_t fontSize   = FONT_SIZE                  * renderScale;
        float    leftMargin = BUTTON_TEXT_LEFT_MARGIN    * renderScale;
        float    topMargin  = BUTTON_TEXT_TOP_MARGIN     * renderScale;
        float    textOffset = TEXT_OFFSET_PER_HOVER_TICK * renderScale          * hoverTicks + 1.0f; // min 1 px

        buttonText = std::make_unique<sf::Text>();
        buttonText->setFont(*Game::instance().getFontManager()->getRegularFont());
        buttonText->setString(text);
        buttonText->setCharacterSize(fontSize);
        buttonText->setFillColor(sf::Color(255, 255, 255, textAlpha));
        buttonText->setPosition(x + leftMargin + textOffset, y + topMargin);
    }
    
    void MainMenuButton::update() {
        Button::update();

        if (enabled && isMouseOver()) {
            if (hoverTicks < MAX_EFFECTIVE_HOVER_TICKS)
                hoverTicks++;
        } else if (hoverTicks > 0)
            hoverTicks--;

        // Вертикальная черта слева от текста
        uint32_t lineAlpha = LINE_ALPHA_PER_HOVER_TICK * hoverTicks;
        line->setFillColor(sf::Color(255, 255, 255, lineAlpha));

        // Закрашивание кнопки
        uint32_t fillAlpha = FILL_ALPHA_PER_HOVER_TICK * hoverTicks;
        fill->setFillColor(sf::Color(0, 0, 0, fillAlpha));

        // Текст на кнопке
        uint32_t textAlpha  = BASE_TEXT_ALPHA + BONUS_TEXT_ALPHA_PER_HOVER_TICK * hoverTicks;
        float    leftMargin = BUTTON_TEXT_LEFT_MARGIN    * renderScale;
        float    topMargin  = BUTTON_TEXT_TOP_MARGIN     * renderScale;
        float    textOffset = TEXT_OFFSET_PER_HOVER_TICK * renderScale          * hoverTicks + 1.0f; // min 1 px

        buttonText->setFillColor(sf::Color(255, 255, 255, textAlpha));
        buttonText->setPosition(x + leftMargin + textOffset, y + topMargin);
    }
    
    void MainMenuButton::draw() {
        Button::draw();

        window->draw(*line);
        window->draw(*fill);
        window->draw(*buttonText);
    }

}
