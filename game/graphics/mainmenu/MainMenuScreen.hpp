#pragma once


#include "../common/Button.hpp"
#include "../common/Screen.hpp"

namespace awd::game {

    class MainMenuScreen : public Screen {
    private:
        std::vector<std::shared_ptr<Button>> buttons;

        void createButtons();
        void addButton(const std::shared_ptr<Button>& button);

        static void createLobbyClicked(Drawable* mainMenuScreen);
        static void createLobbyNextClicked(Drawable* mainMenuScreen, const std::wstring& userInput);
        static void joinLobbyClicked(Drawable* mainMenuScreen);
        static void joinLobbyNextClicked(Drawable* mainMenuScreen, const std::wstring& userInput);
        static void quitGameClicked(Drawable* mainMenuScreen);

    public:
        MainMenuScreen(float renderScale,
                       const std::shared_ptr<sf::RenderWindow>& window);

        void update() override;
        void draw() override;
    };

}

