#pragma once


#include "../common/Button.hpp"
#include "../common/Dialog.hpp"
#include "../common/Screen.hpp"
#include "LobbyScreenListener.hpp"

namespace awd::game {

    class LobbyScreen : public Screen {
    private:
        std::shared_ptr<LobbyScreenListener> listener = std::make_shared<LobbyScreenListener>();

        void createButtons();

    public:
        LobbyScreen(float renderScale,
                    const std::shared_ptr<sf::RenderWindow>& window);

        void update() override;
        void draw() override;

        std::shared_ptr<LobbyScreenListener> getListener() const;
    };

}

