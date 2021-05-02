#define MAX_PLAYER_NAME_LENGTH 15


#include <iostream>
#include "MainMenuScreenListener.hpp"
#include "MainMenuScreen.hpp"
#include "../common/TextInputDialog.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void MainMenuScreenListener::createLobbyClicked(Drawable* mainMenuScreen) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        auto dialog = std::make_shared<TextInputDialog>(
                ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY,
                mainMenu->getRenderScale(),
                mainMenu->getWindow(),
                mainMenu->getListener(),
                mainMenu,
                ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY_TEXT_FIELD_PLAYER_NAME,
                MAX_PLAYER_NAME_LENGTH,
                L"Как вас зовут?",
                ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY_BUTTON_NEXT,
                mainMenu->getListener(),
                ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY_BUTTON_BACK,
                mainMenu->getListener()
        );

        mainMenu->addChild(dialog);
        dialog->show();
    }

    void MainMenuScreenListener::createLobbyNextClicked(Drawable* dialog,
                                                        const std::wstring& playerName) {
        std::cout << "CreateLobby : NEXT : playerName = \"" << playerName.c_str() << "\"" << std::endl;
    }

    void MainMenuScreenListener::createLobbyBackClicked(Drawable* dialog) {
        ((Dialog*) dialog)->hide();
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void MainMenuScreenListener::dialogOpened(Drawable* mainMenuScreen, id_type dialogId) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        mainMenu->setDialogOpen(true);
    }

    void MainMenuScreenListener::dialogClosed(Drawable* mainMenuScreen, id_type dialogId) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        mainMenu->setDialogOpen(false);
        mainMenu->removeChild(dialogId);
    }

    void MainMenuScreenListener::buttonClicked(Drawable* buttonParent, id_type buttonId,
                                               const std::wstring& payload) {
        switch (buttonId) {
            // MainMenu.CreateLobby
            case ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY_BUTTON_NEXT:
                createLobbyNextClicked(buttonParent, payload);
                break;

            // ???
            default:
                std::cerr << "Unhandled button click: buttonId=" << buttonId << std::endl;
                break;
        }
    }

    void MainMenuScreenListener::buttonClicked(Drawable* buttonParent, id_type buttonId,
                                               const NoPayload& payload) {
        switch (buttonId) {
            // MainMenu
            case ID_SCREEN_MAIN_MENU_BUTTON_QUIT_GAME:
                //todo
                break;

            case ID_SCREEN_MAIN_MENU_BUTTON_JOIN_LOBBY:
                //todo
                break;

            case ID_SCREEN_MAIN_MENU_BUTTON_CREATE_LOBBY:
                createLobbyClicked(buttonParent);
                break;

            // MainMenu.CreateLobby
            case ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY_BUTTON_BACK:
                createLobbyBackClicked(buttonParent);
                break;

            // ???
            default:
                std::cerr << "Unhandled button click: buttonId=" << buttonId << std::endl;
                break;
        }
    }

}
