// Кнопки
#define BUTTONS_LEFT_MARGIN 15.0f
#define BUTTONS_HORIZONTAL_MARGIN 15.0f
#define BUTTONS_BOTTOM_MARGIN 15.0f
#define BUTTONS_WIDTH 350.0f
#define BUTTONS_HEIGHT 75.0f
#define BUTTONS_FONT_SIZE 40
// Карточки игроков
#define PLAYER_CARDS_WIDTH 400.0f
#define PLAYER_CARDS_FONT_SIZE 30
// Разделительная линия над кнопками
#define BUTTONS_SEPARATOR_LINE_HEIGHT 5.0f


#include "LobbyScreen.hpp"
#include "../common/WaterBackground.hpp"
#include "../../Game.hpp"
#include "../common/TextButton.hpp"
#include "../common/ColorSet.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void LobbyScreen::createButtons() {
        uint32_t    bFontSize     = BUTTONS_FONT_SIZE         * renderScale;
        const float bLeftMargin   = BUTTONS_LEFT_MARGIN       * renderScale;
        const float bHorizMargin  = BUTTONS_HORIZONTAL_MARGIN * renderScale;
        const float bBottomMargin = BUTTONS_BOTTOM_MARGIN     * renderScale;
        const float bWidth        = BUTTONS_WIDTH             * renderScale;
        const float bHeight       = BUTTONS_HEIGHT            * renderScale;
        buttonsY                  = height - bHeight - bBottomMargin;

        // LeaveLobby
        float bX = bLeftMargin;
        auto btnLeaveLobby = std::make_shared<TextButton>(
                ID_SCREEN_LOBBY_BUTTON_LEAVE_LOBBY, renderScale, window,
                L"Покинуть комнату", bX, buttonsY, bWidth, bHeight, bFontSize, listener);
        addComponent(btnLeaveLobby);

        // NewGame
        bX += bWidth + bHorizMargin;
        auto btnNewGame = std::make_shared<TextButton>(
                ID_SCREEN_LOBBY_BUTTON_NEW_GAME, renderScale, window,
                L"Новая игра", bX, buttonsY, bWidth, bHeight, bFontSize, listener);
        addComponent(btnNewGame);

        // LoadGame
        bX += bWidth + bHorizMargin;
        auto btnLoadGame = std::make_shared<TextButton>(
                ID_SCREEN_LOBBY_BUTTON_LOAD_GAME, renderScale, window,
                L"Загрузить игру", bX, buttonsY, bWidth, bHeight, bFontSize, listener);
        addComponent(btnLoadGame);
    }

    void LobbyScreen::createPlayerCards() {
        uint32_t    cardFontSize = PLAYER_CARDS_FONT_SIZE * renderScale;
        const float cardWidth    = PLAYER_CARDS_WIDTH     * renderScale;
        const float cardHeight   = 2.0f * cardWidth;
        const float cardMargin   = (width - 4.0f * cardWidth) / 5.0f; // 4 - число карточек; 5 - это же число плюс один
        const float cardY        = height / 2.0f - cardHeight / 2.0f;

        float cardX = cardMargin;

        for (int i = 0; i < 4; i++) { // 4 - число карточек
            addComponent(std::make_unique<PlayerCard>(
                    (i + 1) * ID_SCREEN_LOBBY_PLAYER_CARD,
                    renderScale, window,
                    cardX, cardY,
                    cardWidth, cardHeight,
                    listener, cardFontSize
            ));

            cardX += cardWidth + cardMargin;
        }
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    LobbyScreen::LobbyScreen(float renderScale,
                             const std::shared_ptr<sf::RenderWindow>& window)
                             : Screen(ID_SCREEN_LOBBY, renderScale, window) {
        x      = 0.0f;
        y      = 0.0f;
        width  = window->getSize().x;
        height = window->getSize().y;

        // Фон
        addChild(std::make_shared<WaterBackground>(
                ID_SCREEN_LOBBY_WATER_BACKGROUND, renderScale, window));

        auto lobby = Game::instance().getCurrentLobby();
        //todo

        // Кнопки
        createButtons();

        // Карточки игроков
        createPlayerCards();

        // Разделительная линия над кнопками
        float sepMarginX = BUTTONS_LEFT_MARGIN           * renderScale;
        float sepMarginY = BUTTONS_BOTTOM_MARGIN         * renderScale;
        float sepHeight  = BUTTONS_SEPARATOR_LINE_HEIGHT * renderScale + 1.0f; // min 1 px

        sepLine = std::make_unique<sf::RectangleShape>(
                sf::Vector2f(width - 2.0f * sepMarginX, sepHeight));

        sepLine->setFillColor(ColorSet::GUI_LOBBY_SCREEN_BUTTONS_SEPARATOR_LINE);
        sepLine->setPosition(x + sepMarginX, buttonsY - sepHeight - sepMarginY);
    }

    void LobbyScreen::update() {
        Screen::update();
    }

    void LobbyScreen::draw() {
        Screen::draw();

        window->draw(*sepLine);
    }

    std::shared_ptr<LobbyScreenListener> LobbyScreen::getListener() const {
        return listener;
    }

}
