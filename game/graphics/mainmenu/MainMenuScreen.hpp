#pragma once


#include "../Drawable.hpp"

namespace awd::game {

    class MainMenuScreen : public Drawable {
    private:
        void createButtons();

        static void createLobbyClicked();
        static void joinLobbyClicked();
        static void quitGameClicked();

    public:
        MainMenuScreen(float renderScale,
                       const std::shared_ptr<sf::RenderWindow>& window);

        void update() override;
        void draw() override;
    };

}

