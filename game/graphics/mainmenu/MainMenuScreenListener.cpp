#define CREATE_OR_JOIN_TIMEOUT_MILLIS 5000
#define MAX_PLAYER_NAME_LENGTH 15
#define MAX_LOBBY_ID_LENGTH 10 /* чуть больше двух миллиардов --> не более 10 знаков */


#include <iostream>
#include "MainMenuScreenListener.hpp"
#include "MainMenuScreen.hpp"
#include "../common/TextInputDialog.hpp"
#include "../../Game.hpp"
#include "../lobby/LobbyScreen.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    //////////////////////////////////////////////////////////////////////////////////////////////
    //   Утилити-методы
    //////////////////////////////////////////////////////////////////////////////////////////////

    void MainMenuScreenListener::requestPlayerNameInput(Drawable* mainMenuScreen) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        auto dialog = std::make_shared<TextInputDialog>(
                ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT,
                mainMenu->getListener(),
                L"{GRAY}Под этим именем вас будут видеть все другие игроки в комнате. "
                L"Имя может состоять из {WHITE}латинских букв, цифр и символа подчёркивания{GRAY}.",
                ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_TEXT_FIELD,
                mainMenu->getListener(),
                MAX_PLAYER_NAME_LENGTH,
                L"Как вас зовут?",
                mainMenu->getListener()->getEnteredPlayerName(),
                ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_BUTTON_NEXT,
                mainMenu->getListener(),
                ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_BUTTON_BACK,
                mainMenu->getListener()
        );

        mainMenu->openDialog(dialog);
    }

    void MainMenuScreenListener::requestLobbyIdInput(Drawable* mainMenuScreen) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        auto dialog = std::make_shared<TextInputDialog>(
                ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT,
                mainMenu->getListener(),
                L"{GRAY}Этот идентификатор вам должен был сообщить создатель комнаты.",
                ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_TEXT_FIELD,
                mainMenu->getListener(),
                MAX_LOBBY_ID_LENGTH,
                L"Введите идентификатор комнаты",
                mainMenu->getListener()->getEnteredLobbyId(),
                ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_BUTTON_NEXT,
                mainMenu->getListener(),
                ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_BUTTON_BACK,
                mainMenu->getListener()
        );

        mainMenu->openDialog(dialog);
    }

    void MainMenuScreenListener::beginCreateLobby(Drawable* mainMenuScreen,
                                                  const std::wstring& playerName) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        mainMenu->showLoadingOverlay(L"Создание комнаты...", CREATE_OR_JOIN_TIMEOUT_MILLIS);
        Game::instance().getNetService()->createLobbyRequest(playerName);
    }

    void MainMenuScreenListener::beginJoinLobby(Drawable* mainMenuScreen,
                                                uint32_t lobbyId,
                                                const std::wstring& playerName) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        mainMenu->showLoadingOverlay(L"Присоединение к комнате...", CREATE_OR_JOIN_TIMEOUT_MILLIS);
        Game::instance().getNetService()->joinLobbyRequest(lobbyId, playerName);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //   Обработчики событий
    //////////////////////////////////////////////////////////////////////////////////////////////

    void MainMenuScreenListener::quitGameClicked(Drawable* mainMenuScreen) {
        Game::instance().shutdown();
    }

    void MainMenuScreenListener::createLobbyClicked(Drawable* mainMenuScreen) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        mainMenu->setWorkflowState(WorkflowState::CREATING_LOBBY_1);

        // Запрашиваем имя.
        requestPlayerNameInput(mainMenu);
    }

    void MainMenuScreenListener::createLobbyNextClicked(Drawable* dialog) {
        auto* mainMenu = (MainMenuScreen*) dialog->getParent();

        if (!mainMenu->getListener()->getEnteredPlayerName().empty()) {
            // Начинаем процесс создания комнаты.
            mainMenu->setWorkflowState(WorkflowState::CREATING_LOBBY_2);
            mainMenu->closeCurrentDialog();
        }
    }

    void MainMenuScreenListener::createLobbyBackClicked(Drawable* dialog) {
        auto mainMenu = (MainMenuScreen*) dialog->getParent();
        mainMenu->setWorkflowState(WorkflowState::IDLE);
        mainMenu->closeCurrentDialog();
    }

    void MainMenuScreenListener::joinLobbyClicked(Drawable* mainMenuScreen) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        mainMenu->setWorkflowState(WorkflowState::JOINING_LOBBY_1);

        // Сначала запрашиваем ID комнаты.
        requestLobbyIdInput(mainMenu);
    }

    void MainMenuScreenListener::joinLobbyNextClicked(Drawable* dialog) {
        auto mainMenu = (MainMenuScreen*) dialog->getParent();
        WorkflowState workflowState = mainMenu->getWorkflowState();

        if (workflowState == WorkflowState::JOINING_LOBBY_1) {
            if (!mainMenu->getListener()->getEnteredLobbyId().empty()) {
                // Теперь запрашиваем имя. Закрываем этот диалог. Когда закончится анимация
                // его закрытия, откроется новый (см. реализацию метода dialogClosed).
                mainMenu->setWorkflowState(WorkflowState::JOINING_LOBBY_2);
                mainMenu->closeCurrentDialog(); // закрываем диалог ввода ID комнаты
            }
        } else if (workflowState == WorkflowState::JOINING_LOBBY_2) {
            if (!mainMenu->getListener()->getEnteredPlayerName().empty()) {
                // Начинаем процесс присоединения к комнате.
                mainMenu->setWorkflowState(WorkflowState::JOINING_LOBBY_3);
                mainMenu->closeCurrentDialog();
            }
        }
    }

    void MainMenuScreenListener::joinLobbyBackClicked(Drawable* dialog) {
        auto mainMenu = (MainMenuScreen*) dialog->getParent();
        WorkflowState workflowState = mainMenu->getWorkflowState();

        if (workflowState == WorkflowState::JOINING_LOBBY_1) {
            // Выходим обратно в главное меню.
            mainMenu->setWorkflowState(WorkflowState::IDLE);
            mainMenu->closeCurrentDialog(); // закрываем диалог ввода ID комнаты
        } else if (workflowState == WorkflowState::JOINING_LOBBY_2) {
            // Возвращаемся на этап ввода ID комнаты.
            mainMenu->setWorkflowState(WorkflowState::JOINING_LOBBY_1);
            mainMenu->closeCurrentDialog(); // закрываем диалог ввода имени
        }
    }

    void MainMenuScreenListener::errorOkClicked(Drawable* dialog) {
        auto mainMenu = (MainMenuScreen*) dialog->getParent();
        WorkflowState workflowState = mainMenu->getWorkflowState();

        if (workflowState == WorkflowState::IDLE)
            // Закрываем игру.
            Game::instance().shutdown();
        else {
            // Просто закрываем диалог ошибки.
            mainMenu->setWorkflowState(WorkflowState::IDLE);
            mainMenu->closeCurrentDialog();
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    //   Геттеры
    //////////////////////////////////////////////////////////////////////////////////////////////

    std::wstring MainMenuScreenListener::getEnteredPlayerName() const {
        return enteredPlayerName;
    }

    std::wstring MainMenuScreenListener::getEnteredLobbyId() const {
        return enteredLobbyId;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void MainMenuScreenListener::finishCreateLobby(Drawable* mainMenuScreen,
                                                   const std::shared_ptr<net::CreateLobbyResponse>& response) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        mainMenu->hideCurrentLoadingOverlay();

        if (response->lobby_id() < 0) {
            // Отказано в создании комнаты.
            std::wstring errorMessage;
            int errorCode = response->lobby_id();

            switch (errorCode) {
                case -1:
                    errorMessage = L"недопустимое внутриигровое имя";
                    break;

                case -999:
                    errorMessage = L"непредвиденная ошибка на сервере";
                    break;

                default:
                    errorMessage = L"что-то пошло не так (код ошибки: {WHITE}"
                            + std::to_wstring(errorCode) + L"{GRAY})";

                    break;
            }

            mainMenu->showErrorDialog(
                    L"{RED}{BOLD}Отказано в создании комнаты: {RESET}{GRAY}" + errorMessage + L".");
        } else {
            // Комната создана успешно.
            auto lobby = std::make_shared<Lobby>();

            lobby->lobbyId      = response->lobby_id();
            lobby->hostId       = response->player_id();
            lobby->ownPlayerId  = response->player_id();
            lobby->ownCharacter = response->character();
            lobby->playerNames     [response->player_id()] = mainMenu->getListener()->getEnteredPlayerName();
            lobby->playerCharacters[response->player_id()] = response->character();

            Game::instance().setCurrentLobby(lobby);
            Game::instance().setCurrentScreen(std::make_shared<LobbyScreen>());
        }
    }

    void MainMenuScreenListener::finishJoinLobby(Drawable* mainMenuScreen,
                                                 const std::shared_ptr<net::JoinLobbyResponse>& response) {
        auto* mainMenu = (MainMenuScreen*) mainMenuScreen;
        mainMenu->hideCurrentLoadingOverlay();

        if (response->player_id() < 0) {
            // Отказано в присоединении к комнате.
            std::wstring errorMessage;
            int errorCode = response->player_id();

            switch (errorCode) {
                case -1:
                    errorMessage = L"недопустимое внутриигровое имя";
                    break;

                case -2:
                    errorMessage = L"комната переполнена";
                    break;

                case -3:
                    errorMessage = L"вы уже состоите в {WHITE}этой{GRAY} комнате";
                    break;

                case -4:
                    errorMessage = L"вы уже состоите в {WHITE}другой{GRAY} комнате";
                    break;

                case -5:
                    errorMessage = L"это имя уже занято в этой комнате, пожалуйста, выберите другое";
                    break;

                case -6:
                    errorMessage = L"комнаты с указанным идентификатором не существует";
                    break;

                case -999:
                    errorMessage = L"непредвиденная ошибка на сервере";
                    break;

                default:
                    errorMessage = L"что-то пошло не так (код ошибки: {WHITE}"
                                   + std::to_wstring(errorCode) + L"{GRAY})";

                    break;
            }

            mainMenu->showErrorDialog(
                    L"{RED}{BOLD}Отказано в присоединении к комнате: {RESET}{GRAY}" + errorMessage + L".");
        } else {
            // Присоединились к комнате успешно.
            auto lobby = std::make_shared<Lobby>();

            lobby->lobbyId      = std::stoi(mainMenu->getListener()->getEnteredLobbyId());
            lobby->hostId       = response->host_id();
            lobby->ownPlayerId  = response->player_id();
            lobby->ownCharacter = response->character();
            lobby->playerNames     [response->player_id()] = mainMenu->getListener()->getEnteredPlayerName();
            lobby->playerCharacters[response->player_id()] = response->character();

            for (const auto& [otherPlayerId, otherPlayerName] : response->others_names())
                lobby->playerNames[otherPlayerId] = std::wstring(
                        otherPlayerName.begin(), otherPlayerName.end());

            for (const auto& [otherPlayerId, otherPlayerCharacter] : response->others_characters())
                lobby->playerCharacters[otherPlayerId] = otherPlayerCharacter;

            Game::instance().setCurrentLobby(lobby);
            Game::instance().setCurrentScreen(std::make_shared<LobbyScreen>());
        }
    }

    void MainMenuScreenListener::dialogOpened(Drawable* parentScreen, id_type dialogId) {}

    void MainMenuScreenListener::dialogClosed(Drawable* parentScreen, id_type dialogId) {
        auto* mainMenu = (MainMenuScreen*) parentScreen;
        WorkflowState workflowState = mainMenu->getWorkflowState();
        mainMenu->dialogClosed(); // для удаления диалога из списка потомков

        switch (workflowState) {
            default:
                // Ничего не делаем (просто закрылось окно с некритической ошибкой).
                mainMenu->setWorkflowState(WorkflowState::IDLE);
                break;

            case WorkflowState::CREATING_LOBBY_2:
                // Переходим к созданию комнаты.
                mainMenu->setWorkflowState(WorkflowState::WAITING);
                beginCreateLobby(mainMenu, enteredPlayerName);

                break;

            case WorkflowState::JOINING_LOBBY_1:
                // Закрылся диалог ввода имени по кнопке "Назад". Снова запрашиваем ввод имени.
                requestLobbyIdInput(mainMenu);
                break;

            case WorkflowState::JOINING_LOBBY_2:
                // Закрылся диалог ввода ID комнаты. При этом если бы он закрылся кнопкой "Назад",
                // то workflowState был бы IDLE. Значит, он закрылся именно по кнопке "Далее".
                // Переходим к следующему этапу присоединения к комнате - запрашиваем имя.
                requestPlayerNameInput(mainMenu);
                break;

            case WorkflowState::JOINING_LOBBY_3:
                // Все данные для присоединения к комнате введены. Проверяем их и делаем запрос на сервер.
                // WorkflowState обновляем сразу же здесь, чтобы в случае ошибки std::stoi пользователь получил
                // такое же сообщение об ошибке, как и от сервера.
                mainMenu->setWorkflowState(WorkflowState::WAITING);
                uint32_t lobbyId;

                try {
                    // Переходим к присоединению к комнате. (Если введённые данные корректны для std::stoi.)
                    lobbyId = std::stoi(enteredLobbyId);
                    beginJoinLobby(mainMenu, lobbyId, enteredPlayerName);
                } catch (const std::invalid_argument&) {
                    // Введено что-то, отличное от целого числа.
                    mainMenu->showErrorDialog(L"{RED}{BOLD}Ошибка ввода: "
                                              L"{RESET}{GRAY}идентификатор комнаты должен быть "
                                              L"{WHITE}целым неотрицательным{GRAY} числом (причём не очень большим).");
                } catch (const std::out_of_range&) {
                    // Введено слишком большое целое число.
                    mainMenu->showErrorDialog(L"{RED}{BOLD}Ошибка ввода: "
                                              L"{RESET}{GRAY}идентификатор комнаты должен быть "
                                              L"{WHITE}целым неотрицательным{GRAY} числом (причём не очень большим).");
                }

                break;
        }
    }

    void MainMenuScreenListener::buttonClicked(Drawable* buttonParent, id_type buttonId) {
        WorkflowState workflowState;

        switch (buttonId) {
            // MainMenu
            case ID_SCREEN_MAIN_MENU_BUTTON_QUIT_GAME:
                quitGameClicked(buttonParent);
                break;

            case ID_SCREEN_MAIN_MENU_BUTTON_JOIN_LOBBY:
                joinLobbyClicked(buttonParent);
                break;

            case ID_SCREEN_MAIN_MENU_BUTTON_CREATE_LOBBY:
                createLobbyClicked(buttonParent);
                break;

            // MainMenu.CreateLobby + MainMenu.JoinLobby
            case ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_BUTTON_NEXT:
                workflowState = ((MainMenuScreen*) buttonParent->getParent())->getWorkflowState();

                if (workflowState == WorkflowState::CREATING_LOBBY_1)
                    createLobbyNextClicked(buttonParent);
                else if (workflowState == WorkflowState::JOINING_LOBBY_1
                      || workflowState == WorkflowState::JOINING_LOBBY_2)
                    joinLobbyNextClicked(buttonParent);

                break;

            case ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_BUTTON_BACK:
                workflowState = ((MainMenuScreen*) buttonParent->getParent())->getWorkflowState();

                if (workflowState == WorkflowState::CREATING_LOBBY_1)
                    createLobbyBackClicked(buttonParent);
                else if (workflowState == WorkflowState::JOINING_LOBBY_1
                      || workflowState == WorkflowState::JOINING_LOBBY_2)
                    joinLobbyBackClicked(buttonParent);

                break;

            // MainMenu.Error
            case ID_SCREEN_MAIN_MENU_DIALOG_ERROR_BUTTON_OK:
                errorOkClicked(buttonParent);
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
        auto* mainMenu = (MainMenuScreen*) textFieldParent->getParent();
        WorkflowState workflowState = mainMenu->getWorkflowState();

        if (workflowState == WorkflowState::CREATING_LOBBY_1
                || workflowState == WorkflowState::JOINING_LOBBY_2)
            enteredPlayerName = newContents;
        else if (workflowState == WorkflowState::JOINING_LOBBY_1)
            enteredLobbyId = newContents;
    }

}
