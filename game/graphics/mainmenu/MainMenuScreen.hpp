#pragma once


#include "../common/Button.hpp"
#include "../common/Drawable.hpp"
#include "../common/Dialog.hpp"

namespace awd::game {

    class MainMenuScreen : public Drawable {
    private:
        bool dialogOpen = false;

        void createButtons();

        static void dialogOpened(Drawable* mainMenuScreen, int dialogId);
        static void dialogClosed(Drawable* mainMenuScreen, int dialogId);
        static void createLobbyClicked(Drawable* mainMenuScreen);
        static void createLobbyNextClicked(Drawable* mainMenuScreen, const std::wstring& userInput);
        static void createLobbyBackClicked(Drawable* mainMenuScreen);
        static void joinLobbyClicked(Drawable* mainMenuScreen);
        static void joinLobbyNextClicked(Drawable* mainMenuScreen, const std::wstring& userInput);
        static void joinLobbyBackClicked(Drawable* mainMenuScreen);
        static void quitGameClicked(Drawable* mainMenuScreen);

    public:
        MainMenuScreen(float renderScale,
                       const std::shared_ptr<sf::RenderWindow>& window);

        void update() override;
        void draw() override;
    };

}

