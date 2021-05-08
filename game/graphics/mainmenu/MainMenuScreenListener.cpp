#define MAX_PLAYER_NAME_LENGTH 15
#define MAX_LOBBY_ID_LENGTH 10 /* чуть больше двух миллиардов --> не более 10 знаков */


#include <iostream>
#include "MainMenuScreenListener.hpp"
#include "MainMenuScreen.hpp"
#include "../common/TextInputDialog.hpp"
#include "../../Game.hpp"

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
                mainMenu->getRenderScale(),
                mainMenu->getWindow(),
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
                mainMenu->getRenderScale(),
                mainMenu->getWindow(),
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

    void MainMenuScreenListener::displayInputError(Drawable* mainMenuScreen,
                                                   const std::wstring& errMsg) {

    }

    void MainMenuScreenListener::beginCreateLobby(Drawable* mainMenuScreen,
                                                  const std::wstring& playerName) {

    }

    void MainMenuScreenListener::beginJoinLobby(Drawable* mainMenuScreen,
                                                uint32_t lobbyId,
                                                const std::wstring& playerName) {
        //todo
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
        std::wstring playerName = mainMenu->getListener()->getEnteredPlayerName();

        if (!playerName.empty())
            // Переходим к созданию комнаты.
            beginCreateLobby(mainMenu, playerName);
    }

    void MainMenuScreenListener::createLobbyBackClicked(Drawable* dialog) {
        auto mainMenu = (MainMenuScreen*) dialog->getParent();
        mainMenu->setWorkflowState(WorkflowState::IDLE);
        ((Dialog*) dialog)->hide();
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
            if (!mainMenu->getListener()->getEnteredPlayerName().empty()) {
                // Теперь запрашиваем имя. Закрываем этот диалог. Когда закончится анимация
                // его закрытия, откроется новый (см. реализацию метода dialogClosed).
                mainMenu->setWorkflowState(WorkflowState::JOINING_LOBBY_2);
                mainMenu->closeCurrentDialog(); // закрываем диалог ввода ID комнаты
            }
        } else {
            std::wstring lobbyIdStr = mainMenu->getListener()->getEnteredLobbyId();
            uint32_t lobbyId;

            try {
                lobbyId = std::stoi(lobbyIdStr);
            } catch (const std::invalid_argument&) {
                displayInputError(mainMenu, L"{RED}{BOLD}Ошибка ввода: "
                                            L"{RESET}{GRAY}идентификатор комнаты должен быть "
                                            L"{WHITE}целым неотрицательным{GRAY} числом (причём не очень большим).");
            } catch (const std::out_of_range&) {
                displayInputError(mainMenu, L"{RED}{BOLD}Ошибка ввода: "
                                            L"{RESET}{GRAY}идентификатор комнаты должен быть "
                                            L"{WHITE}целым неотрицательным{GRAY} числом (причём не очень большим).");
            }

            // Переходим к присоединению к комнате.
            beginJoinLobby(mainMenu, lobbyId, mainMenu->getListener()->getEnteredPlayerName());
        }
    }

    void MainMenuScreenListener::joinLobbyBackClicked(Drawable* dialog) {
        auto mainMenu = (MainMenuScreen*) dialog->getParent();
        WorkflowState workflowState = mainMenu->getWorkflowState();
        ((Dialog*) dialog)->hide();

        if (workflowState == WorkflowState::JOINING_LOBBY_1)
            // Выходим обратно в главное меню (закрываем диалог).
            mainMenu->setWorkflowState(WorkflowState::IDLE);
        else {
            // Возвращаемся на этап ввода ID комнаты.
            mainMenu->setWorkflowState(WorkflowState::JOINING_LOBBY_1);
            mainMenu->closeCurrentDialog(); // закрываем диалог ввода имени
        }
    }

    void MainMenuScreenListener::errorOkClicked(Drawable* dialog) {
        Game::instance().shutdown();
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void MainMenuScreenListener::dialogOpened(Drawable* parentScreen, id_type dialogId) {}

    void MainMenuScreenListener::dialogClosed(Drawable* parentScreen, id_type dialogId) {
        auto* mainMenu = (MainMenuScreen*) parentScreen;
        WorkflowState workflowState = mainMenu->getWorkflowState();
        mainMenu->dialogClosed(); // для удаления диалога из списка потомков

        if (workflowState == WorkflowState::JOINING_LOBBY_1)
            // Закрылся диалог ввода имени. Снова запрашиваем ввод имени.
            requestLobbyIdInput(mainMenu);
        else if (workflowState == WorkflowState::JOINING_LOBBY_2)
            // Закрылся диалог ввода ID комнаты. При этом если бы он закрылся кнопкой "Назад",
            // то workflowState был бы IDLE. Значит, он закрылся именно по кнопке "Далее".
            // Переходим к следующему этапу присоединения к комнате - запрашиваем имя.
            requestPlayerNameInput(mainMenu);
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
                else
                    joinLobbyNextClicked(buttonParent);

                break;

            case ID_SCREEN_MAIN_MENU_DIALOG_TEXT_INPUT_BUTTON_BACK:
                workflowState = ((MainMenuScreen*) buttonParent->getParent())->getWorkflowState();

                if (workflowState == WorkflowState::CREATING_LOBBY_1)
                    createLobbyBackClicked(buttonParent);
                else
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

    std::wstring MainMenuScreenListener::getEnteredPlayerName() const {
        return enteredPlayerName;
    }

    std::wstring MainMenuScreenListener::getEnteredLobbyId() const {
        return enteredLobbyId;
    }

}
