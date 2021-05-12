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
#include "graphics/common/Screen.hpp"
#include "lobby/Lobby.hpp"
#include "graphics/common/TextureManager.hpp"

namespace awd::game {

    class Game {
    private:
        uint32_t currentTick = 0;

        std::shared_ptr<FontManager>    fonts    = std::make_shared<FontManager>   ();
        std::shared_ptr<TextureManager> textures = std::make_shared<TextureManager>();

        std::shared_ptr<net::UdpClient>      udpClient         = nullptr;
        std::shared_ptr<net::PacketManager>  packetManager     = nullptr;
        std::shared_ptr<net::NetworkService> netService        = nullptr;
        uint32_t                             lastRtt           = 0;

        bool gameWindowFocused = true;

        std::shared_ptr<Lobby> currentLobby             = nullptr;
        std::atomic<bool>      shuttingDown             = false;
        std::atomic<int>       currentState             = GameState::LOBBY;

        float                             renderScale   = 0.0f;
        std::shared_ptr<sf::RenderWindow> window        = nullptr;
        std::shared_ptr<Screen>           currentScreen = nullptr;

        Game();
        ~Game(); // NOLINT(modernize-use-equals-delete)

        bool loadAssets();
        void registerPacketListeners();
        void startGameLoop();
        void runGameLoop(uint32_t tickDelay, sf::Clock& tickClock);
        void update();
        void render();
        void postScreenLoad();
        void flushPackets();

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
        void handshakeComplete(uint32_t serverProtocolVersion);
        void socketBindFailed();

        uint32_t getCurrentTick() const;

        std::shared_ptr<FontManager>    getFonts   () const;
        std::shared_ptr<TextureManager> getTextures() const;

        std::shared_ptr<net::PacketManager>  getPacketManager    () const;
        std::shared_ptr<net::NetworkService> getNetService       () const;
        uint32_t                             getLastRtt          () const;
        void                                 setLastRtt(uint32_t rtt);

        std::shared_ptr<Lobby> getCurrentLobby() const;
        void                   setCurrentLobby(const std::shared_ptr<Lobby>& lobby);
        uint32_t               getCurrentState() const;

        float                             getRenderScale  () const;
        std::shared_ptr<sf::RenderWindow> getWindow       () const;
        std::shared_ptr<Screen>           getCurrentScreen() const;
        void                              setCurrentScreen(const std::shared_ptr<Screen>& screen);

        // TODO переместить эти методы в какой-то утилити-класс
        static uint32_t randUInt(uint32_t min, uint32_t max);
        static float randFloat(float min, float max);
    };

}
