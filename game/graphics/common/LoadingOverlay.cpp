// Текст
#define MAX_EFFECTIVE_EXISTED_TICKS 20
#define TEXT_BASE_ALPHA 75
#define TEXT_BONUS_ALPHA_PER_EXISTED_TICK 8
#define TEXT_FONT_SIZE 50
#define TEXT_OUTLINE_WIDTH 2
#define TEXT_TOP_MARGIN 50


#include "LoadingOverlay.hpp"
#include "../../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    LoadingOverlay::LoadingOverlay(id_type id,
                                   float renderScale,
                                   const std::shared_ptr<sf::RenderWindow>& window,
                                   const std::wstring& message)
            : Drawable(id, renderScale, window) {
        // Текст
        unsigned int fontSize  = TEXT_FONT_SIZE     * renderScale;
        unsigned int outline   = TEXT_OUTLINE_WIDTH * renderScale + 1; // min 1 px
        unsigned int topMargin = TEXT_TOP_MARGIN    * renderScale;

        text.setFont(*Game::instance().getFontManager()->getRegularFont());
        text.setCharacterSize(fontSize);
        text.setOutlineThickness(outline);
        text.setString(message);

        sf::FloatRect textBounds = text.getGlobalBounds();

        unsigned int textX = window->getSize().x / 2 - textBounds.width / 2;
        unsigned int textY = textBounds.height + topMargin;
        text.setPosition(textX, textY);
    }

    void LoadingOverlay::update() {
        Drawable::update();

        if (existedTicks < MAX_EFFECTIVE_EXISTED_TICKS)
            existedTicks++;
    }

    void LoadingOverlay::draw() {
        Drawable::draw();

        // Текст
        unsigned int textAlpha = TEXT_BASE_ALPHA + TEXT_BONUS_ALPHA_PER_EXISTED_TICK * existedTicks;
        text.setFillColor   (sf::Color(255, 255, 255, textAlpha));
        text.setOutlineColor(sf::Color(  0,   0,   0, textAlpha));

        window->draw(text);
    }

}
