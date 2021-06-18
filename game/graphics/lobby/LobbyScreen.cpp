// Кнопки
#define BUTTONS_LEFT_MARGIN 15.0f
#define BUTTONS_HORIZONTAL_MARGIN 15.0f
#define BUTTONS_BOTTOM_MARGIN 15.0f
#define BUTTONS_WIDTH 350.0f
#define BUTTONS_HEIGHT 75.0f
#define BUTTONS_FONT_SIZE 40
// Карточки игроков
#define BASE_PLAYER_TEXTURE_WIDTH 22.0f
#define BASE_PLAYER_TEXTURE_HEIGHT 34.0f
#define PLAYER_CARDS_SCALE 18.0f /* в 18 раз больше базовой модельки */
#define PLAYER_CARDS_WIDTH BASE_PLAYER_TEXTURE_WIDTH * PLAYER_CARDS_SCALE
#define PLAYER_CARDS_HEIGHT BASE_PLAYER_TEXTURE_HEIGHT * PLAYER_CARDS_SCALE
#define PLAYER_CARDS_FONT_SIZE 27
// Разделительная линия над кнопками
#define BUTTONS_SEPARATOR_LINE_HEIGHT 3.0f
// Надпись с ID комнаты
#define LOBBY_ID_TEXT_FONT_SIZE 45
#define LOBBY_ID_TEXT_RIGHT_MARGIN 30.0f
#define LOBBY_ID_TEXT_BOTTOM_MARGIN 45.0f
#define LOBBY_ID_TEXT_COPIED_DISPLAY_TICKS 25


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
                ID_SCREEN_LOBBY_BUTTON_LEAVE_LOBBY, L"Покинуть комнату",
                bX, buttonsY, bWidth, bHeight, bFontSize, listener);
        addComponent(btnLeaveLobby);

        if (isHost) {
            // NewGame
            bX += bWidth + bHorizMargin;
            auto btnNewGame = std::make_shared<TextButton>(
                    ID_SCREEN_LOBBY_BUTTON_NEW_GAME, L"Новая игра",
                    bX, buttonsY, bWidth, bHeight, bFontSize, listener);
            addComponent(btnNewGame);

            // LoadGame
            bX += bWidth + bHorizMargin;
            auto btnLoadGame = std::make_shared<TextButton>(
                    ID_SCREEN_LOBBY_BUTTON_LOAD_GAME, L"Загрузить игру",
                    bX, buttonsY, bWidth, bHeight, bFontSize, listener);
            addComponent(btnLoadGame);
        }
    }

    void LobbyScreen::createPlayerCards() {
        uint32_t    cardFontSize   = PLAYER_CARDS_FONT_SIZE   * renderScale;
        const float cardWidth      = PLAYER_CARDS_WIDTH       * renderScale;
        const float cardHeight     = PLAYER_CARDS_HEIGHT      * renderScale;
        const float cardMargin     = (width - 4.0f * cardWidth) / 5.0f; // делим на 5, т.к. "дырок" - 5
        const float cardY          = sepLine->getPosition().y / 2.0f - cardHeight / 2.0f;

        float cardX = cardMargin;

        for (int i = 1; i <= 4; i++) { // 4 - число карточек
            id_type cardId = i + ID_SCREEN_LOBBY_PLAYER_CARD;

            auto card = std::make_shared<PlayerCard>(
                    cardId,
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

    LobbyScreen::LobbyScreen() : Screen(ID_SCREEN_LOBBY) {
        auto lobby = Game::instance().getCurrentLobby();

        // Фон
        addChild(std::make_shared<WaterBackground>(ID_SCREEN_LOBBY_WATER_BACKGROUND));

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

    void LobbyScreen::mousePressed(const sf::Event::MouseButtonEvent& event) {
        Screen::mousePressed(event);

        // Клик по надписи "Комната #xxxxxx" для копирования идентификатора комнаты в буфер обмена.
        float mouseX            = event.x; // NOLINT(cppcoreguidelines-narrowing-conversions)
        float mouseY            = event.y; // NOLINT(cppcoreguidelines-narrowing-conversions)
        float lobbyIdTextX      = lobbyIdText->getPosition().x;
        float lobbyIdTextY      = lobbyIdText->getPosition().y;
        float lobbyIdTextWidth  = lobbyIdText->getGlobalBounds().width;
        float lobbyIdTextHeight = lobbyIdText->getGlobalBounds().height;

        if (mouseX >= lobbyIdTextX && mouseX <= (lobbyIdTextX + lobbyIdTextWidth)
                && mouseY >= lobbyIdTextY && mouseY <= (lobbyIdTextY + lobbyIdTextHeight)) {
            uint32_t lobbyId = Game::instance().getCurrentLobby()->lobbyId;
            sf::Clipboard::setString(std::to_string(lobbyId));
            labelCopiedDisplayTicks = LOBBY_ID_TEXT_COPIED_DISPLAY_TICKS;
            lobbyIdText->setString(L"* ID СКОПИРОВАН *");
        }
    }

    void LobbyScreen::update() {
        Screen::update();

        if (playerCards.size() != 4)
            // Экран лобби уже закрылся/закрывается.
            return;

        // Надпись с ID комнаты
        if (labelCopiedDisplayTicks > 0)
            labelCopiedDisplayTicks--;
        else {
            uint32_t lobbyId = Game::instance().getCurrentLobby()->lobbyId;
            lobbyIdText->setString(L"Комната #" + std::to_wstring(lobbyId));
        }

        // Карточки игроков
        auto lobby = Game::instance().getCurrentLobby();
        uint32_t cardNum = 0; // 0..3

        for (const auto& [playerId, playerName] : lobby->playerNames) {
            auto card = playerCards[cardNum];
            bool isSelf = playerId == lobby->ownPlayerId;

            std::wstring text = playerName;

            if (playerId == lobby->hostId)
                text += L" {GRAY}(хост)";

            if (isSelf)
                text += L" {GOLD}(вы)";

            text = StringUtils::encodeFormatting(text);

            card->updateHighlighted(isSelf);
            card->updateDisplayedCharacter(lobby->playerCharacters[playerId]);
            card->updateDisplayedText(text);

            cardNum++;
        }

        while (cardNum < 4) { // 4 персонажа (тут мы создаём местозаполнители, если нужно)
            auto card = playerCards[cardNum];
            card->updateHighlighted(false);
            card->updateDisplayedCharacter(0);
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
                ID_SCREEN_LOBBY_LOADING_OVERLAY, loadingMessage);

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
                listener,
                message,
                ID_SCREEN_LOBBY_DIALOG_ERROR_BUTTON_OK,
                listener
        );

        openDialog(dialog);
    }

}
