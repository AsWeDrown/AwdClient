// Кнопки
#define BUTTONS_LEFT_MARGIN 15.0f
#define BUTTONS_HORIZONTAL_MARGIN 15.0f
#define BUTTONS_BOTTOM_MARGIN 15.0f
#define BUTTONS_WIDTH 350.0f
#define BUTTONS_HEIGHT 75.0f
#define BUTTONS_FONT_SIZE 40
// Карточки игроков
#define PLAYER_CARDS_WIDTH 400.0f
#define PLAYER_CARDS_FONT_SIZE 27
// Разделительная линия над кнопками
#define BUTTONS_SEPARATOR_LINE_HEIGHT 3.0f
// Надпись с ID комнаты
#define LOBBY_ID_TEXT_FONT_SIZE 45
#define LOBBY_ID_TEXT_RIGHT_MARGIN 30.0f
#define LOBBY_ID_TEXT_BOTTOM_MARGIN 45.0f


#include <thread>
#include "LobbyScreen.hpp"
#include "../common/WaterBackground.hpp"
#include "../../Game.hpp"
#include "../common/TextButton.hpp"
#include "../common/ColorSet.hpp"
#include "../../util/StringUtils.hpp"
#include "../common/ErrorDialog.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void LobbyScreen::createButtons(bool isHost) {
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

        if (isHost) {
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
    }

    void LobbyScreen::createPlayerCards() {
        uint32_t    cardFontSize   = PLAYER_CARDS_FONT_SIZE   * renderScale;
        const float cardWidth      = PLAYER_CARDS_WIDTH       * renderScale;
        const float cardHeight     = 2.0f * cardWidth; // базовые текстуры - 16 x 32 (W x 2*W)
        const float cardMargin     = (width - 4.0f * cardWidth) / 5.0f; // делим на 5, т.к. "дырок" - 5
        const float cardY          = sepLine->getPosition().y / 2.0f - cardHeight / 2.0f;

        float cardX = cardMargin;

        for (int i = 1; i <= 4; i++) { // 4 - число карточек
            id_type cardId = ID_SCREEN_LOBBY + i * ID_OTHER;

            auto card = std::make_shared<PlayerCard>(
                    cardId,
                    renderScale, window,
                    cardX, cardY,
                    cardWidth, cardHeight,
                    listener, cardFontSize
            );

            addComponent(card);
            playerCards.push_back(card);
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

        auto lobby = Game::instance().getCurrentLobby();

        // Фон
        addChild(std::make_shared<WaterBackground>(
                ID_SCREEN_LOBBY_WATER_BACKGROUND, renderScale, window));

        // Кнопки
        createButtons(lobby->ownPlayerId == lobby->hostId);

        // Разделительная линия над кнопками
        float sepMarginX = BUTTONS_LEFT_MARGIN           * renderScale;
        float sepMarginY = BUTTONS_BOTTOM_MARGIN         * renderScale;
        float sepHeight  = BUTTONS_SEPARATOR_LINE_HEIGHT * renderScale + 1.0f; // min 1 px

        sepLine = std::make_unique<sf::RectangleShape>(
                sf::Vector2f(width - 2.0f * sepMarginX, sepHeight));

        sepLine->setFillColor(ColorSet::GUI_LOBBY_SCREEN_BUTTONS_SEPARATOR_LINE);
        sepLine->setPosition(x + sepMarginX, buttonsY - sepHeight - sepMarginY);

        // Надпись с ID комнаты
        uint32_t idFontSize     = LOBBY_ID_TEXT_FONT_SIZE     * renderScale;
        float    idRightMargin  = LOBBY_ID_TEXT_RIGHT_MARGIN  * renderScale;
        float    idBottomMargin = LOBBY_ID_TEXT_BOTTOM_MARGIN * renderScale;

        lobbyIdText = std::make_unique<sf::Text>();
        lobbyIdText->setFont(*Game::instance().getFonts()->regularFont);
        lobbyIdText->setCharacterSize(idFontSize);
        lobbyIdText->setString(L"Комната #" + std::to_wstring(lobby->lobbyId));
        lobbyIdText->setFillColor(sf::Color::White);

        sf::FloatRect idBounds = lobbyIdText->getGlobalBounds();

        lobbyIdText->setPosition(
                width  - idBounds.width  - idRightMargin,
                height - idBounds.height - idBottomMargin
        );

        // Карточки игроков
        createPlayerCards();
    }

    void LobbyScreen::update() {
        Screen::update();

        // Карточки игроков
        auto lobby = Game::instance().getCurrentLobby();
        uint32_t cardNum = 1; // 1..4

        for (const auto& [playerId, playerName] : lobby->playerNames) {
            auto card = playerCards[cardNum - 1];
            bool isSelf = playerId == lobby->ownPlayerId;

            std::wstring text = playerName;

            if (playerId == lobby->hostId)
                text += L" {GRAY}(хост)";

            if (isSelf)
                text += L" {GOLD}(вы)";

            text = StringUtils::encodeFormatting(text);

            card->setHighlighted(isSelf);
            card->setDisplayedCharacter(lobby->playerCharacters[playerId]);
            card->setDisplayedText(text);

            cardNum++;
        }

        while (cardNum <= 4) { // 4 персонажа (тут мы создаём местозаполнители, если нужно)
            auto card = playerCards[cardNum - 1];
            card->setDisplayedCharacter(0);
            cardNum++;
        }
    }

    void LobbyScreen::draw() {
        Screen::draw();

        window->draw(*sepLine);
        window->draw(*lobbyIdText);
    }

    std::shared_ptr<LobbyScreenListener> LobbyScreen::getListener() const {
        return listener;
    }

    void LobbyScreen::showLoadingOverlay(const std::wstring& loadingMessage,
                                         uint32_t timeoutMillis) {
        auto loadingOverlay = std::make_shared<LoadingOverlay>(
                ID_SCREEN_LOBBY_LOADING_OVERLAY,
                renderScale, window, loadingMessage);

        Screen::showLoadingOverlay(loadingOverlay);

        std::thread timeoutThread([this, loadingMessage, timeoutMillis]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeoutMillis));

            if (this != Game::instance().getCurrentScreen().get())
                // За это время поменялся экран. Код дальше крашнет игру.
                return;

            auto currentLoadingOverlay = getChildById(currentLoadingOverlayId);

            if (currentLoadingOverlay != nullptr) {
                std::wstring currentLoadingMessage = std::dynamic_pointer_cast
                        <LoadingOverlay>(currentLoadingOverlay)->getUnformattedMessage();

                if (currentLoadingMessage == loadingMessage) {
                    // Истекло время ожидания (hideCurrentLoadingOverlay
                    // для ЭТОГО сообщения загрузки так и не был вызван).
                    hideCurrentLoadingOverlay(); // скрываем сообщение о загрузке (оно больше не нужно), ...
                    closeCurrentDialog(); // ... закрываем текущий диалог, каким бы он не был, ...

                    auto dialog = std::make_shared<ErrorDialog>(
                            ID_SCREEN_LOBBY_DIALOG_ERROR,
                            renderScale,
                            window,
                            listener,
                            L"{RED}{BOLD}Ошибка: "
                            L"{RESET}{GRAY}истекло максимальное допустимое время ожидания. "
                            L"Проверьте своё подключение к Интернету.",
                            ID_SCREEN_LOBBY_DIALOG_ERROR_BUTTON_OK,
                            listener
                    );

                    openDialog(dialog); // ... и отображаем диалог с ошибкой об истечении времени ожидания.
                }
            }
        });

        timeoutThread.detach();
    }

    void LobbyScreen::showErrorDialog(const std::wstring& message) {
        auto dialog = std::make_shared<ErrorDialog>(
                ID_SCREEN_LOBBY_DIALOG_ERROR,
                renderScale,
                window,
                listener,
                message,
                ID_SCREEN_LOBBY_DIALOG_ERROR_BUTTON_OK,
                listener
        );

        openDialog(dialog);
    }

}
