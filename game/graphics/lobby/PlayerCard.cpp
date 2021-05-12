// Общий фон + контур
#define CARD_RECT_OUTLINE_WIDTH 7.0f
// Карточки игроков (заполненные)
#define CHARACTER_SPRITE_HORIZONTAL_MARGIN 20.0f
#define CHARACTER_SPRITE_VERTICAL_MARGIN 2.0f * CHARACTER_SPRITE_HORIZONTAL_MARGIN
#define CHARACTER_PANEL_HEIGHT_PART 0.1f
#define CHARACTER_PANEL_ALPHA 175
#define CHARACTER_TEXT_BOTTOM_MARGIN 25.0f


#include "PlayerCard.hpp"
#include "../../Game.hpp"
#include "../common/ColorSet.hpp"
#include "../../util/RenderUtils.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    PlayerCard::PlayerCard(id_type id,
                           float x, float y,
                           float width, float height,
                           const std::shared_ptr<ButtonListener>& listener,
                           uint32_t fontSize)
                           : Button(id, listener) {
        this->x        = x;
        this->y        = y;
        this->width    = width;
        this->height   = height;
        this->fontSize = fontSize;

        // Фон + контур
        float outlineWidth = CARD_RECT_OUTLINE_WIDTH * renderScale;

        cardRect = std::make_unique<sf::RectangleShape>(
                sf::Vector2f(width - 2.0f * outlineWidth, height - 2.0f * outlineWidth));

        cardRect->setFillColor(ColorSet::GUI_LOBBY_SCREEN_CARD_RECT_FILL);
        cardRect->setOutlineColor(ColorSet::GUI_LOBBY_SCREEN_CARD_RECT_OUTLINE);
        cardRect->setOutlineThickness(outlineWidth);
        cardRect->setPosition(x + outlineWidth, y + outlineWidth);
    }

    void PlayerCard::update() {
        Button::update();
    }

    void PlayerCard::draw() {
        Button::draw();

        window->draw(*cardRect);

        if (displayedCharacter == 0)
            // Рисуем местозаполнитель
            window->draw(*placeholderText);
        else if (displayedCharacter != 9999) {
            // Рисуем спрайт персонажа
            window->draw(*characterSprite);
            window->draw(*characterPanel);
            window->draw(*characterText);
        }
    }

    void PlayerCard::setHighlighted(bool isHighlighted) {
        if (this->highlighted != isHighlighted) {
            this->highlighted = isHighlighted;

            // Обновляем цвет (выделение самого себя).
            if (isHighlighted) {
                cardRect->setFillColor(ColorSet::GUI_LOBBY_SCREEN_CARD_RECT_FILL_HIGHLIGHTED);
                cardRect->setOutlineColor(ColorSet::GUI_LOBBY_SCREEN_CARD_RECT_OUTLINE_HIGHLIGHTED);
            } else {
                cardRect->setFillColor(ColorSet::GUI_LOBBY_SCREEN_CARD_RECT_FILL);
                cardRect->setOutlineColor(ColorSet::GUI_LOBBY_SCREEN_CARD_RECT_OUTLINE);
            }
        }
    }

    uint32_t PlayerCard::getDisplayedCharacter() const {
        return displayedCharacter;
    }

    void PlayerCard::setDisplayedCharacter(uint32_t character) {
        if (this->displayedCharacter != character) {
            this->displayedCharacter = character;

            if (character == 0) {
                // Рисуем местозаполнитель (свободное место для игрока).
                characterSprite = nullptr;
                characterPanel  = nullptr;
                characterText   = nullptr;

                // Текст
                placeholderText = std::make_unique<sf::Text>();
                placeholderText->setFont(*Game::instance().getFonts()->regularFont);
                placeholderText->setCharacterSize(2 * fontSize);
                placeholderText->setFillColor(ColorSet::GUI_LOBBY_SCREEN_PLACEHOLDER_TEXT);
                placeholderText->setString(L"Свободная касса");

                sf::FloatRect textBounds = placeholderText->getGlobalBounds();
                placeholderText->setPosition(
                        x + width  / 2.0f - textBounds.width  / 2.0f,
                        y + height / 2.0f - textBounds.height / 2.0f
                );
            } else {
                // Рисуем карточку игрока.
                placeholderText = nullptr;

                // Спрайт персонажа
                float spriteHorMargin = CHARACTER_SPRITE_HORIZONTAL_MARGIN * renderScale;
                float spriteVerMargin = CHARACTER_SPRITE_VERTICAL_MARGIN   * renderScale;
                float spriteWidth     = width  - 2.0f * spriteHorMargin;
                float spriteHeight    = height - 2.0f * spriteVerMargin;

                characterSprite = std::make_unique<sf::Sprite>();

                characterSprite->setPosition(x + spriteHorMargin, y + spriteVerMargin);
                characterSprite->setTexture(*Game::instance().getTextures()->characters[character - 1]);

                sf::FloatRect baseBounds = characterSprite->getGlobalBounds();
                characterSprite->setScale( // устанавливаем нужный нам размер спрайта (из 16x32 --> WxH)
                        spriteWidth  / baseBounds.width,
                        spriteHeight / baseBounds.height
                );

                // Панелька, поверх которой будет текст
                float panelHeight = height * CHARACTER_PANEL_HEIGHT_PART;

                characterPanel = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, panelHeight));
                characterPanel->setFillColor(sf::Color(0, 0, 0, CHARACTER_PANEL_ALPHA));
                characterPanel->setPosition(x, y + height - panelHeight);

                // Текст
                characterText = std::make_unique<sfe::RichText>(
                        *Game::instance().getFonts()->regularFont);
            }
        }
    }

    void PlayerCard::setDisplayedText(const std::wstring& text) {
        if (this->displayedText != text) {
            this->displayedText = text;

            if (displayedCharacter != 0) { // не рисуем этот текст на карточках-местозаполнтелях - у них есть свой.
                RenderUtils::enrichText(*characterText, text);
                characterText->setCharacterSize(fontSize);

                sf::FloatRect textBounds = characterText->getGlobalBounds();
                float bottomMargin = CHARACTER_TEXT_BOTTOM_MARGIN * renderScale;

                characterText->setPosition(
                        x + width  / 2.0f - textBounds.width  / 2.0f,
                        y + height - textBounds.height - bottomMargin
                );
            }
        }
    }

}
