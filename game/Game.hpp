#pragma once


#include <SFML/Graphics.hpp>
#include <memory>
#include "../net/UdpClient.hpp"
#include "../net/PacketManager.hpp"
#include "screen/Screen.hpp"

namespace awd::game {

    class Game {
    private:
        std::shared_ptr<net::UdpClient> udpClient = nullptr;
        std::shared_ptr<net::PacketManager> packetManager = nullptr;
        std::shared_ptr<sf::RenderWindow> window = nullptr;
        std::shared_ptr<game::Screen> currentScreen = nullptr;

        Game();
        ~Game(); // NOLINT(modernize-use-equals-delete)

        void registerPacketListeners();
        void startGameLoop();
        void runGameLoop();
        void update();
        void render();

    public:
        static Game& instance() {
            // "согласно стандарту, этот код ленивый и потокобезопасный"
            // - https://ru.stackoverflow.com/a/327139
            static Game instance;
            return instance;
        }

        Game(Game const&) = delete;
        Game& operator =(Game const&) = delete;

        void bootstrap();

        std::shared_ptr<net::PacketManager> getPacketManager() const;
        sf::Vector2u getScreenSize() const;
    };

}
