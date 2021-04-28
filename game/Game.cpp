#define HOST "julia.reflex.rip"
#define PORT 23132
#define GAME_TPS 20


#include <thread>
#include "Game.hpp"
#include "packetlistener/HandshakeResponseListener.hpp"
#include "packetlistener/CreateLobbyResponseListener.hpp"
#include "screen/mainmenu/MainMenuScreen.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *   PRIVATE
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void Game::registerPacketListeners() {
        packetManager->registerListener(
                net::PacketWrapper::PacketCase::kHandshakeResponse,
                std::make_shared<HandshakeResponseListener>()
        );

        packetManager->registerListener(
                net::PacketWrapper::PacketCase::kCreateLobbyResponse,
                std::make_shared<CreateLobbyResponseListener>()
        );
    }

    void Game::startGameLoop() {
        auto bestVideoMode = sf::VideoMode::getFullscreenModes()[0];
        window = std::make_shared<sf::RenderWindow>(
                bestVideoMode, "As We Drown"/*, sf::Style::Fullscreen*/);

        window->setFramerateLimit(120);
        currentScreen = std::make_shared<game::MainMenuScreen>(window);

        auto tickDelay = std::chrono::milliseconds(1000 / GAME_TPS);

        while (window->isOpen()) {
            runGameLoop();
            std::this_thread::sleep_for(tickDelay);
        }

        // todo tearDown
    }

    void Game::runGameLoop() {
        sf::Event event; // NOLINT(cppcoreguidelines-pro-type-member-init)

        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window->close();
                    break;
            }
        }

        update();
        window->clear();
        render();
        window->display();
    }

    void Game::update() {

    }

    void Game::render() {
        currentScreen->render();
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *   PUBLIC
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Game::Game() {
        udpClient = std::make_shared<net::UdpClient>(HOST, PORT);
        packetManager = std::make_shared<net::PacketManager>(udpClient);
    }

    Game::~Game() = default;

    void Game::bootstrap() {
        registerPacketListeners();
        udpClient->startInNewThread();
        startGameLoop();
    }

    std::shared_ptr<net::PacketManager> Game::getPacketManager() const {
        return packetManager;
    }

    sf::Vector2u Game::getScreenSize() const {
        return window->getSize();
    }

}
