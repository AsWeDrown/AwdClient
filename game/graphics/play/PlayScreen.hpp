#pragma once


#include "../common/Screen.hpp"
#include "PlayScreenListener.hpp"
#include "../../world/World.hpp"
#include "QuestManager.hpp"
#include "Chat.hpp"
#include "Environment.hpp"

namespace awd::game {

    class PlayScreen : public Screen {
    private:
        bool justJoined = true; // используется для уведомления сервера о присоединении к миру (игровому экрану)

        std::shared_ptr<PlayScreenListener> listener = std::make_shared<PlayScreenListener>();

        // Создаются в конструкторе:
        std::shared_ptr<World> world = nullptr;
        std::shared_ptr<QuestManager> questManager = nullptr;
        std::shared_ptr<Chat> chat = nullptr;
        std::shared_ptr<Environment> environment = nullptr;
        // ... (SFML) ...
        std::unique_ptr<sf::Text> performanceHud = nullptr;
        std::unique_ptr<sf::RectangleShape> alarmRect = nullptr;

        uint32_t currentAlarmAlpha = 0;
        bool reducingAlarmAlpha = false;

        void updateEnvironment();

        void updateAlarm();

    public:
        PlayScreen();

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        std::shared_ptr<PlayScreenListener> getListener() const;
        std::shared_ptr<World> getWorld() const;
        std::shared_ptr<QuestManager> getQuestManager() const;
        std::shared_ptr<Chat> getChat() const;
        std::shared_ptr<Environment> getEnvironment() const;
    };

}

