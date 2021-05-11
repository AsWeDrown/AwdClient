#pragma once


#include "MainMenuScreenListener.hpp"
#include "../common/Screen.hpp"

namespace awd::game {

    enum class WorkflowState {
        IDLE,             // ничего не делаем

        CREATING_LOBBY_1, // ждём ввода имени для создания комнаты
        CREATING_LOBBY_2, // всё готово, ждём закрытия диалога, чтобы отправить запрос серверу
        CREATING_LOBBY_3, // запрос отправили - ждём ответ от сервера

        JOINING_LOBBY_1,  // ждём ввода ID комнаты для присоединения к комнате
        JOINING_LOBBY_2,  // ждём ввода имени для присоединения к комнате (ID комнаты уже введён)
        JOINING_LOBBY_3,  // всё готово, ждём закрытия диалога, чтобы отправить запрос серверу
        JOINING_LOBBY_4   // запрос отправили - ждём ответ от сервера
    };

    class MainMenuScreen : public Screen {
    private:
        float highestButtonY = 0.0f;

        std::shared_ptr<MainMenuScreenListener> listener = std::make_shared<MainMenuScreenListener>();
        WorkflowState workflowState = WorkflowState::IDLE;

        std::unique_ptr<sf::Text> logo = nullptr;

        void createButtons();

    public:
        MainMenuScreen(float renderScale,
                       const std::shared_ptr<sf::RenderWindow>& window);

        void update() override;
        void draw() override;

        std::shared_ptr<MainMenuScreenListener> getListener() const;
        WorkflowState getWorkflowState() const;
        void setWorkflowState(WorkflowState state);

        void showLoadingOverlay(const std::wstring& loadingMessage,
                                uint32_t timeoutMillis);

        void showErrorDialog(const std::wstring& message);
    };

}

