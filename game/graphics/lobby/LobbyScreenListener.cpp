#define LEAVE_TIMEOUT_MILLIS 5000
#define NEW_OR_LOAD_GAME_TIMEOUT_MILLIS 7500
#define PLAY_STATE_LOAD_TIMEOUT_MILLIS 30000


#include <thread>
#include <iostream>
#include "LobbyScreenListener.hpp"
#include "LobbyScreen.hpp"
#include "../../Game.hpp"
#include "../mainmenu/MainMenuScreen.hpp"

namespace awd::game {

    //////////////////////////////////////////////////////////////////////////////////////////////
    //   Утилити-методы
    //////////////////////////////////////////////////////////////////////////////////////////////

    void LobbyScreenListener::beginLeaveLobby(Drawable* lobbyDrawable) {
        auto* lobbyScreen = (LobbyScreen*) lobbyDrawable;
        lobbyScreen->hideCurrentLoadingOverlay();
        lobbyScreen->showLoadingOverlay(L"Выход из комнаты...", LEAVE_TIMEOUT_MILLIS);
        Game::instance().getNetService()->leaveLobbyRequest();
    }

    void LobbyScreenListener::beginNewGame(Drawable* lobbyDrawable) {
        auto* lobbyScreen = (LobbyScreen*) lobbyDrawable;
        lobbyScreen->hideCurrentLoadingOverlay();
        lobbyScreen->showLoadingOverlay(L"Игра скоро начнётся...", NEW_OR_LOAD_GAME_TIMEOUT_MILLIS);
        Game::instance().getNetService()->beginPlayStateRequest("0");
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //   Обработчики событий
    //////////////////////////////////////////////////////////////////////////////////////////////

    void LobbyScreenListener::leaveLobbyClicked(Drawable* lobbyDrawable) {
        auto currentLobby = Game::instance().getCurrentLobby();

        if (currentLobby != nullptr)
            beginLeaveLobby(lobbyDrawable);
    }

    void LobbyScreenListener::newGameClicked(Drawable* lobbyDrawable) {
        auto currentLobby = Game::instance().getCurrentLobby();

        if (currentLobby != nullptr)
            beginNewGame(lobbyDrawable);
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
            Game::instance().setCurrentScreen(std::make_shared<MainMenuScreen>());
        } else
            // Ошибка.
            lobbyScreen->showErrorDialog(
                    L"{RED}{BOLD}Что-то пошло не так: {RESET}{GRAY}код ошибки: {WHITE}"
                    + std::to_wstring(response->status_code()) + L"{GRAY}.");
    }

    void LobbyScreenListener::kickedFromLobby(Drawable* lobbyDrawable,
                                              const std::shared_ptr<net::KickedFromLobby>& kick) {
        auto currentLobby = Game::instance().getCurrentLobby();

        if (currentLobby != nullptr) {
            auto mainMenuScreen = std::make_shared<MainMenuScreen>();

            Game::instance().setCurrentScreen(mainMenuScreen);
            Game::instance().setCurrentLobby(nullptr);

            std::wstring kickReason;

            switch (kick->reason()) {
                case 1:
                    kickReason = L"комната была расформирована (её хост вышел, "
                                 L"либо игра не начиналась слишком долго)";
                    break;

                default:
                    kickReason = L"что-то пошло не так (код причины: {WHITE} "
                                 + std::to_wstring(kick->reason()) + L"{GRAY};";

                    break;
            }

            mainMenuScreen->setWorkflowState(WorkflowState::WAITING); // чтобы игра не закрылась при нажатии "ОК"
            mainMenuScreen->showErrorDialog(
                    L"{RED}{BOLD}Вы были исключены из комнаты:{RESET}{GRAY} " + kickReason);
        }
    }

    void LobbyScreenListener::finishBeginPlayState(Drawable* lobbyDrawable,
                                                   const std::shared_ptr<net::BeginPlayStateResponse>& response) {
        auto* lobbyScreen = (LobbyScreen*) lobbyDrawable;

        // Если == 1 -> игра запускается успешно. Ждём, пока сервер передаст нам и остальным
        // игрокам в комнате всю необходимую для старта игровой стадии информацию (мир и т.д.).
        if (response->status_code() != 1) {
            // Ошибка.
            std::wstring errorMessage;
            int errorCode = response->status_code();

            switch (errorCode) {
                case -1:
                    errorMessage = L"недостаточно игроков в комнате для начала";
                    break;

                case -2:
                    errorMessage = L"данных о локальном сохранении нет на сервере "
                                   L"(возможно, локальное сохранение устарело)";
                    break;

                case -3:
                    errorMessage = L"невозможно загрузить сохранение - состав игроков (имена и/или "
                                   L"количество) в комнате отличаются от записанного в сохранении";

                    break;

                case -4:
                    errorMessage = L"локальное сохранение было создано для другой версии игры";
                    break;

                case -999:
                    errorMessage = L"непредвиденная ошибка на сервере";
                    break;

                default:
                    errorMessage = L"что-то пошло не так (код ошибки: {WHITE}"
                                   + std::to_wstring(errorCode) + L"{GRAY})";

                    break;
            }

            lobbyScreen->showErrorDialog(
                    L"{RED}{BOLD}Не удалось начать игру: {RESET}{GRAY}" + errorMessage + L".");
        }
    }

    void LobbyScreenListener::initialUpdateDimension(Drawable* lobbyDrawable,
                                                     const std::shared_ptr<net::UpdateDimensionCommand>& command) {
        auto lobbyScreen = (LobbyScreen*) lobbyDrawable;

        if (lobbyScreen->getListener()->playScreen == nullptr) { // защита от повторного получения пакетов
            // Загружаем указанное сервером измерение и сообщаем ему по завершении,
            // переходя в игровую стадию (PLAY). Делаем это в другом потоке, чтобы
            // не тормозить текущий поток (поток получения/обработки пакетов).
            uint32_t dimension = command->dimension();

            lobbyScreen->getListener()->playScreen = std::make_shared<PlayScreen>();
            lobbyScreen->getListener()->playScreen->getWorld()->updateDimension(dimension);
            Game::instance().setCurrentState(GameState::PLAY);
            Game::instance().getNetService()->updateDimensionComplete();
        }
    }

    void LobbyScreenListener::joinWorld(Drawable* lobbyDrawable,
                                        const std::shared_ptr<net::JoinWorldCommand>& command) {
        auto lobbyScreen = (LobbyScreen*) lobbyDrawable;

        if (!Game::instance().isJoinedWorld()) { // защита от повторного получения пакетов
            // Переходим на игровой экран. Серверу сообщим об этом по завершении:
            // в следующем тике, в методе update() игрового экрана (PlayScreen).
            lobbyScreen->hideCurrentLoadingOverlay();
            Game::instance().setJoinedWorld(true);
            Game::instance().setCurrentScreen(lobbyScreen->getListener()->playScreen);
        }
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

            case ID_SCREEN_LOBBY_BUTTON_NEW_GAME:
                newGameClicked(buttonParent);
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
