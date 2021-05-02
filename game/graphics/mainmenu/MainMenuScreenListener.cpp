#define MAX_PLAYER_NAME_LENGTH 15


#include <iostream>
#include "MainMenuScreenListener.hpp"
#include "MainMenuScreen.hpp"
#include "../common/TextInputDialog.hpp"
#include "../common/ColorSet.hpp"
#include "../../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void MainMenuScreenListener::quitGameClicked(Drawable* mainMenuScreen) {
        Game::instance().shutdown();
    }

    void MainMenuScreenListener::createLobbyClicked(Drawable* mainMenuScreen) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        auto dialog = std::make_shared<TextInputDialog>(
                ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY,
                mainMenu->getRenderScale(),
                mainMenu->getWindow(),
                mainMenu->getListener(),
                L"{GRAY}Под этим именем вас будут видеть все другие игроки в комнате. "
                        L"{YELLOW}{BOLD}Обратите внимание: {RESET}{GRAY}если в будущем вы захотите "
                        L"{WHITE}загрузить сохранение {GRAY}игры в этой комнате, вам потребуется "
                        L"использовать в точности это же имя.",
                ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY_TEXT_FIELD_PLAYER_NAME,
                mainMenu->getListener(),
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

    void MainMenuScreenListener::createLobbyNextClicked(Drawable* dialog) {
        auto* mainMenu = (MainMenuScreen*) dialog->getParent();
        std::wstring playerName = mainMenu->getListener()->getEnteredPlayerName();
        std::wcout << L"CreateLobby : NEXT : playerName = \"" << playerName << L"\"" << std::endl;
        //todo
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

    void MainMenuScreenListener::buttonClicked(Drawable* buttonParent, id_type buttonId) {
        switch (buttonId) {
            // MainMenu
            case ID_SCREEN_MAIN_MENU_BUTTON_QUIT_GAME:
                quitGameClicked(buttonParent);
                break;

            case ID_SCREEN_MAIN_MENU_BUTTON_JOIN_LOBBY:
                //todo
                break;

            case ID_SCREEN_MAIN_MENU_BUTTON_CREATE_LOBBY:
                createLobbyClicked(buttonParent);
                break;

            // MainMenu.CreateLobby
            case ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY_BUTTON_NEXT:
                createLobbyNextClicked(buttonParent);
                break;

            case ID_SCREEN_MAIN_MENU_DIALOG_CREATE_LOBBY_BUTTON_BACK:
                createLobbyBackClicked(buttonParent);
                break;

            // ???
            default:
                std::wcerr << L"Unhandled button click: parent=" << buttonParent->getId()
                          << L", buttonId=" << buttonId << std::endl;
                break;
        }
    }

    void MainMenuScreenListener::contentsChanged(Drawable* textFieldParent, id_type textFieldId,
                                                 const std::wstring& newContents) {
        enteredPlayerName = newContents;
    }

    std::wstring MainMenuScreenListener::getEnteredPlayerName() const {
        return enteredPlayerName;
    }

}