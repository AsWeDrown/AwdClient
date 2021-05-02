#pragma once


#include <SFML/Graphics.hpp>
#include <memory>
#include "../net/UdpClient.hpp"
#include "../net/PacketManager.hpp"
#include "graphics/common/Drawable.hpp"
#include "GameState.hpp"
#include "graphics/common/FontManager.hpp"
#include "FatalErrors.hpp"

namespace awd::game {

    class Game {
    private:
        std::shared_ptr<FontManager> fontManager = nullptr;

        std::shared_ptr<net::UdpClient> udpClient = nullptr;
        std::shared_ptr<net::PacketManager> packetManager = nullptr;

        int currentState = GameState::LOBBY;

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

        std::shared_ptr<FontManager> getFontManager() const;
        std::shared_ptr<net::PacketManager> getPacketManager() const;
        int getCurrentState() const;

        static unsigned int randUInt(unsigned int min, unsigned int max);
        static float randFloat(float min, float max);
    };

}
