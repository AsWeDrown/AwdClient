#pragma once


#include "../common/Screen.hpp"
#include "LobbyScreenListener.hpp"
#include "PlayerCard.hpp"

namespace awd::game {

    class LobbyScreen : public Screen {
    private:
        float buttonsY = 0.0f;

        std::shared_ptr<LobbyScreenListener> listener = std::make_shared<LobbyScreenListener>();

        std::vector<std::shared_ptr<PlayerCard>> playerCards;

        std::unique_ptr<sf::RectangleShape> sepLine    = nullptr;
        std::unique_ptr<sf::Text>           lobbyIdText = nullptr;

        void createButtons(bool isHost);
        void createPlayerCards();

    public:
        LobbyScreen();

        void update() override;
        void draw() override;

        std::shared_ptr<LobbyScreenListener> getListener() const;

        void showLoadingOverlay(const std::wstring& loadingMessage,
                                uint32_t timeoutMillis);

        void showErrorDialog(const std::wstring& message);
    };

}

