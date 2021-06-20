#pragma once


#include <SFML/Graphics.hpp>
#include <memory>
#include "../net/UdpClient.hpp"
#include "../net/PacketManager.hpp"
#include "graphics/common/Drawable.hpp"
#include "GameState.hpp"
#include "graphics/data/FontManager.hpp"
#include "FatalErrors.hpp"
#include "../net/NetworkService.hpp"
#include "graphics/common/Screen.hpp"
#include "lobby/Lobby.hpp"
#include "graphics/data/TextureManager.hpp"
#include "profiling/TpsMeter.hpp"
#include "profiling/FpsMeter.hpp"
#include "world/World.hpp"
#include "config/ConfigManager.hpp"
#include "sound/SoundManager.hpp"
#include "sound/SoundSystem.hpp"

namespace awd::game {

    class Game {
    private:
        std::shared_ptr<game_time> lastFrameNanoTime = nullptr;

        int64_t tickDelayNanos        = 0,
                frameAccumulatorNanos = 0;

        uint32_t currentTick = 0;

        std::shared_ptr<TpsMeter> tpsMeter;
        std::shared_ptr<FpsMeter> fpsMeter;

        std::shared_ptr<ConfigManager>  configs  = std::make_shared<ConfigManager> ();
        std::shared_ptr<FontManager>    fonts    = std::make_shared<FontManager>   ();
        std::shared_ptr<TextureManager> textures = std::make_shared<TextureManager>();
        std::shared_ptr<SoundManager>   sounds   = std::make_shared<SoundManager>  ();

        std::shared_ptr<SoundSystem> soundSystem = std::make_shared<SoundSystem>();

        std::shared_ptr<net::UdpClient>      udpClient         = nullptr;
        std::shared_ptr<net::PacketManager>  packetManager     = nullptr;
        std::shared_ptr<net::NetworkService> netService        = nullptr;
        uint32_t                             lastRtt           = 0;

        bool gameWindowFocused = true;

        std::shared_ptr<Lobby> currentLobby             = nullptr;
        std::atomic<bool>      joinedWorld              = false;
        std::atomic<bool>      shuttingDown             = false;
        std::atomic<uint32_t>  currentState             = GameState::AUTH;

        float                             renderScale   = 0.0f;
        std::shared_ptr<sf::RenderWindow> window        = nullptr;
        std::shared_ptr<Screen>           currentScreen = nullptr;

        Game();
        ~Game(); // NOLINT(modernize-use-equals-delete)

        bool loadAssets();
        void registerPacketListeners();
        void startGameLoop();
        void runGameLoop();
        void update();
        void render();
        void postScreenLoad();

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

        std::shared_ptr<TpsMeter> getTpsMeter() const;
        std::shared_ptr<FpsMeter> getFpsMeter() const;

        std::shared_ptr<ConfigManager>  getConfigs () const;
        std::shared_ptr<FontManager>    getFonts   () const;
        std::shared_ptr<TextureManager> getTextures() const;
        std::shared_ptr<SoundManager>   getSounds  () const;

        std::shared_ptr<SoundSystem> getSoundSystem() const;

        std::shared_ptr<net::PacketManager>  getPacketManager    () const;
        std::shared_ptr<net::NetworkService> getNetService       () const;
        uint32_t                             getLastRtt          () const;
        void                                 setLastRtt(uint32_t rtt);
        bool                                 isGameFocused       () const;

        bool isJoinedWorld() const;
        void setJoinedWorld(bool joined);

        uint32_t getCurrentState() const;
        void     setCurrentState(uint32_t newState);

        float                             getRenderScale  () const;
        std::shared_ptr<sf::RenderWindow> getWindow       () const;
        std::shared_ptr<Screen>           getCurrentScreen() const;
        void                              setCurrentScreen(const std::shared_ptr<Screen>& screen);

        std::shared_ptr<World> currentWorld    () const;
        std::shared_ptr<Lobby> getCurrentLobby () const;
        void                   setCurrentLobby(const std::shared_ptr<Lobby>& lobby);

        void returnToMainMenu();

        // TODO переместить эти методы в какой-то утилити-класс
        static uint32_t randUInt (uint32_t min, uint32_t max);
        static float    randFloat(float    min, float    max);
    };

}
