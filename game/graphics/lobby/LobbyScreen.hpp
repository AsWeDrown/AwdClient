#pragma once


#include "../common/Screen.hpp"
#include "LobbyScreenListener.hpp"
#include "PlayerCard.hpp"

namespace awd::game {

    class LobbyScreen : public Screen {
    private:
        float buttonsY = 0.0f;

        std::shared_ptr<LobbyScreenListener> listener = std::make_shared<LobbyScreenListener>();

        std::unique_ptr<sf::RectangleShape> sepLine = nullptr;

        void createButtons();
        void createPlayerCards();

    public:
        LobbyScreen(float renderScale,
                    const std::shared_ptr<sf::RenderWindow>& window);

        void update() override;
        void draw() override;

        std::shared_ptr<LobbyScreenListener> getListener() const;
    };

}

