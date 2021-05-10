#define LEAVE_TIMEOUT_MILLIS 5000


#include <iostream>
#include "LobbyScreenListener.hpp"
#include "LobbyScreen.hpp"
#include "../../Game.hpp"
#include "../mainmenu/MainMenuScreen.hpp"

namespace awd::game {

    //////////////////////////////////////////////////////////////////////////////////////////////
    //   Утилити-методы
    //////////////////////////////////////////////////////////////////////////////////////////////

    void LobbyScreenListener::beginLeaveLobby(Drawable* lobbyDrawable,
                                              const std::shared_ptr<Lobby>& currentLobby) {
        auto* lobbyScreen = (LobbyScreen*) lobbyDrawable;
        lobbyScreen->showLoadingOverlay(L"Выход из комнаты...", LEAVE_TIMEOUT_MILLIS);

        Game::instance().getNetService()->leaveLobbyRequest(
                currentLobby->lobbyId, currentLobby->ownPlayerId);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //   Обработчики событий
    //////////////////////////////////////////////////////////////////////////////////////////////

    void LobbyScreenListener::leaveLobbyClicked(Drawable* lobbyDrawable) {
        auto currentLobby = Game::instance().getCurrentLobby();

        if (currentLobby != nullptr)
            beginLeaveLobby(lobbyDrawable, currentLobby);
    }

    void LobbyScreenListener::errorOkClicked(Drawable* dialog) {
        auto lobbyScreen = (LobbyScreen*) dialog->getParent();
        lobbyScreen->closeCurrentDialog(); // Просто закрываем диалог ошибки.
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void LobbyScreenListener::finishLeaveLobby(Drawable* lobbyDrawable,
                                               const std::shared_ptr<net::LeaveLobbyResponse>& response) {
        auto* lobbyScreen = (LobbyScreen*) lobbyDrawable;
        lobbyScreen->hideCurrentLoadingOverlay();

        if (response->status_code() == 1) {
            // Успешный выход.
            Game::instance().setCurrentLobby(nullptr);
            Game::instance().setCurrentScreen(std::make_shared
                    <MainMenuScreen>(lobbyScreen->getRenderScale(), lobbyScreen->getWindow()));
        } else
            // Ошибка.
            lobbyScreen->showErrorDialog(
                    L"{RED}{BOLD}Что-то пошло не так: {RESET}{GRAY}код ошибки: {WHITE}"
                    + std::to_wstring(response->status_code()) + L"{GRAY}.");
    }

    void LobbyScreenListener::dialogOpened(Drawable* parentScreen, id_type dialogId) {}

    void LobbyScreenListener::dialogClosed(Drawable* parentScreen, id_type dialogId) {
        auto* lobbyScreen = (LobbyScreen*) parentScreen;
        lobbyScreen->dialogClosed(); // для удаления диалога из списка потомков
    }

    void LobbyScreenListener::buttonClicked(Drawable* buttonParent, id_type buttonId) {
        switch (buttonId) {
            // Lobby
            case ID_SCREEN_LOBBY_BUTTON_LEAVE_LOBBY:
                leaveLobbyClicked(buttonParent);
                break;

            // Lobby.Error
            case ID_SCREEN_LOBBY_DIALOG_ERROR_BUTTON_OK:
                errorOkClicked(buttonParent);
                break;

            // ???
            default:
                std::wcerr << L"Unhandled button click: parent=" << buttonParent->getId()
                           << L", buttonId=" << buttonId << std::endl;

                break;
        }
    }

}
