// Размер и расположения текста на кнопках
#define BUTTON_TEXT_LEFT_MARGIN 25
#define BUTTON_TEXT_TOP_MARGIN 6
#define FONT_SIZE 45
// Параметры выделенных кнопок
#define VERTICAL_LINE_WIDTH 10
#define MAX_EFFECTIVE_HOVER_TICKS 7
#define LINE_ALPHA_PER_HOVER_TICK 35
#define FILL_ALPHA_PER_HOVER_TICK 15
#define BASE_TEXT_ALPHA 175
#define BONUS_TEXT_ALPHA_PER_HOVER_TICK 7
#define TEXT_OFFSET_PER_HOVER_TICK 1


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
                           unsigned int x, unsigned int y,
                           unsigned int width, unsigned int height,
                           const std::shared_ptr<ButtonListener>& listener)
                           : Button(id, renderScale, window, listener) {
        this->text = text;
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }
    
    void TextButton::update() {
        Button::update();

        if (enabled && isMouseOver()) {
            if (hoverTicks < MAX_EFFECTIVE_HOVER_TICKS)
                hoverTicks++;
        } else if (hoverTicks > 0)
            hoverTicks--;
    }
    
    void TextButton::draw() {
        Button::draw();

        // Вертикальная черта слева от текста
        unsigned int lineWidth = VERTICAL_LINE_WIDTH       * renderScale;
        unsigned int lineAlpha = LINE_ALPHA_PER_HOVER_TICK * hoverTicks;

        sf::RectangleShape line(sf::Vector2f(lineWidth, height));
        line.setPosition(x, y);
        line.setFillColor(sf::Color(255, 255, 255, lineAlpha));
        window->draw(line);

        // Закрашивание кнопки
        unsigned int fillAlpha = FILL_ALPHA_PER_HOVER_TICK * hoverTicks;

        sf::RectangleShape fill(sf::Vector2f(width, height));
        fill.setPosition(x + lineWidth, y);
        fill.setFillColor(sf::Color(0, 0, 0, fillAlpha));
        window->draw(fill);

        // Текст
        unsigned int fontSize   = FONT_SIZE                  * renderScale;
        unsigned int leftMargin = BUTTON_TEXT_LEFT_MARGIN    * renderScale;
        unsigned int topMargin  = BUTTON_TEXT_TOP_MARGIN     * renderScale;
        unsigned int textOffset = TEXT_OFFSET_PER_HOVER_TICK * renderScale          * hoverTicks + 1; // min 1 px
        unsigned int textAlpha  = BASE_TEXT_ALPHA + BONUS_TEXT_ALPHA_PER_HOVER_TICK * hoverTicks;

        sf::Text bText;
        bText.setFont(*Game::instance().getFontManager()->getRegularFont());
        bText.setString(text);
        bText.setCharacterSize(fontSize);
        bText.setFillColor(sf::Color(255, 255, 255, textAlpha));
        bText.setPosition(x + leftMargin + textOffset, y + topMargin);
        window->draw(bText);
    }

}
