// Текст
#define MAX_EFFECTIVE_EXISTED_TICKS 20
#define TEXT_BASE_ALPHA 160
#define TEXT_BONUS_ALPHA_PER_EXISTED_TICK 4
#define TEXT_FONT_SIZE 50
#define TEXT_OUTLINE_WIDTH 2.0f
#define TEXT_TOP_MARGIN 50.0f


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
        this->unformattedMessage = message;

        // Текст
        unsigned int fontSize  = TEXT_FONT_SIZE     * renderScale;
        float        outline   = TEXT_OUTLINE_WIDTH * renderScale + 1.0f; // min 1 px
        float        topMargin = TEXT_TOP_MARGIN    * renderScale;

        text = std::make_unique<sf::Text>();
        text->setFont(*Game::instance().getFontManager()->getRegularFont());
        text->setCharacterSize(fontSize);
        text->setOutlineThickness(outline);
        text->setString(message);

        sf::FloatRect textBounds = text->getGlobalBounds();

        float textX = window->getSize().x / 2.0f - textBounds.width / 2.0f;
        float textY = textBounds.height + topMargin;
        text->setPosition(textX, textY);
    }

    void LoadingOverlay::update() {
        Drawable::update();

        // "Пульсация" текста (то ярче, то тусклее).
        if (reverseTick) {
            if (existedTicks > 0)
                existedTicks--;
            else
                reverseTick = false;
        } else {
            if (existedTicks < MAX_EFFECTIVE_EXISTED_TICKS)
                existedTicks++;
            else
                reverseTick = true;
        }

        unsigned int textAlpha = TEXT_BASE_ALPHA + TEXT_BONUS_ALPHA_PER_EXISTED_TICK * existedTicks;
        text->setFillColor   (sf::Color(255, 255, 255, textAlpha));
        text->setOutlineColor(sf::Color(  0,   0,   0, textAlpha));
    }

    void LoadingOverlay::draw() {
        Drawable::draw();
        window->draw(*text);
    }

    std::wstring LoadingOverlay::getUnformattedMessage() const {
        return unformattedMessage;
    }

}
