#pragma once


#include <SFML/Graphics.hpp>
#include <memory>
#include "../net/UdpClient.hpp"
#include "../net/PacketManager.hpp"
#include "graphics/common/Drawable.hpp"
#include "GameState.hpp"
#include "graphics/common/FontManager.hpp"
#include "FatalErrors.hpp"
#include "../net/NetworkService.hpp"

namespace awd::game {

    class Game {
    private:
        std::shared_ptr<FontManager> fontManager = nullptr;

        std::shared_ptr<net::UdpClient> udpClient = nullptr;
        std::shared_ptr<net::PacketManager> packetManager = nullptr;
        std::shared_ptr<net::NetworkService> netService = nullptr;

        std::atomic<bool> shuttingDown = false;
        std::atomic<int> currentState = GameState::LOBBY;

        std::shared_ptr<sf::RenderWindow> window = nullptr;
        std::shared_ptr<Drawable> currentScreen = nullptr;

        Game();
        ~Game(); // NOLINT(modernize-use-equals-delete)

        bool loadAssets();
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
        void shutdown();

        std::shared_ptr<FontManager> getFontManager() const;
        std::shared_ptr<net::PacketManager> getPacketManager() const;
        std::shared_ptr<net::NetworkService> getNetService() const;
        int getCurrentState() const;

        static unsigned int randUInt(unsigned int min, unsigned int max);
        static float randFloat(float min, float max);
    };

}
