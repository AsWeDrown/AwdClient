#define HOST "julia.reflex.rip"
#define PORT 23132
#define GAME_TPS 25
#define BASE_SCREEN_WIDTH 1920
#define BASE_SCREEN_HEIGHT 1080


#include <thread>
#include "Game.hpp"
#include "packetlistener/HandshakeResponseListener.hpp"
#include "packetlistener/CreateLobbyResponseListener.hpp"
#include "graphics/mainmenu/MainMenuScreen.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool Game::loadAssets() {
        std::wcout << L"Loading assets:" << std::endl;

        return fontManager->loadFonts();
    }

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
                bestVideoMode, "As We Drown", sf::Style::Fullscreen);

        float renderScale = std::min((float) bestVideoMode.width  / (float) BASE_SCREEN_WIDTH,
                                     (float) bestVideoMode.height / (float) BASE_SCREEN_HEIGHT);

        std::wcout << L"Video mode: " << bestVideoMode.width << L"x" << bestVideoMode.height
                  << L" (" << bestVideoMode.bitsPerPixel << L" bits per pixel)" << std::endl;
        std::wcout << L"Render scale: " << renderScale << std::endl;

        currentState = GameState::LOBBY;
        currentScreen = std::make_shared<MainMenuScreen>(renderScale, window);

        auto tickDelay = std::chrono::milliseconds(1000 / GAME_TPS);

        while (window->isOpen() && currentState != GameState::EXIT) {
            runGameLoop();
            std::this_thread::sleep_for(tickDelay);
        }

        std::wcout << L"Reached end of game loop." << std::endl;
        shutdown(); // на случай, если выход был произведён, скажем, по крестику, а не через кнопку в главном меню
    }

    void Game::runGameLoop() {
        sf::Event event; // NOLINT(cppcoreguidelines-pro-type-member-init)

        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window->close();
                    break;

                case sf::Event::KeyPressed:
                    currentScreen->keyPressed(event.key);
                    break;

                case sf::Event::MouseButtonPressed:
                    currentScreen->mousePressed(event.mouseButton);
                    break;
            }
        }

        update();
        window->clear();
        render();
        window->display();
    }

    void Game::update() {
        currentScreen->update();
    }

    void Game::render() {
        currentScreen->draw();
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Game::Game() {
        fontManager = std::make_shared<FontManager>();
        udpClient = std::make_shared<net::UdpClient>(HOST, PORT);
        packetManager = std::make_shared<net::PacketManager>(udpClient);
    }

    Game::~Game() = default;

    void Game::bootstrap() {
        // Инициализация ГПСЧ.
        srand(static_cast<unsigned int>(time(nullptr))); // NOLINT(cert-msc51-cpp)

        // Загрузка текстур, звуков, шрифтов и прочего.
        if (!loadAssets()) {
            std::wcerr << L"Failed to load assets." << std::endl;
            return;
        }

        // UDP-клиент (в другом потоке).
        registerPacketListeners();
        udpClient->startInNewThread();

        // Главный цикл игры (в этом потоке). Должен запускаться в самом конце!
        startGameLoop();
    }

    void Game::shutdown() {
        if (shuttingDown)
            return; // игнорируем возможные повторные вызовы

        shuttingDown = true;

        std::wcout << L"Shutting down..." << std::endl;
        udpClient->tearDown();
        currentState = GameState::EXIT;

        std::wcout << L"Bye!" << std::endl;
    }

    std::shared_ptr<FontManager> Game::getFontManager() const {
        return fontManager;
    }

    std::shared_ptr<net::PacketManager> Game::getPacketManager() const {
        return packetManager;
    }

    std::shared_ptr<net::NetworkService> Game::getNetService() const {
        return netService;
    }

    int Game::getCurrentState() const {
        return currentState;
    }

    unsigned int Game::randUInt(unsigned int min, unsigned int max) {
        if (min == max)
            return min;

        if (min > max)
            std::swap(min, max);

        if (max > RAND_MAX)
            max = RAND_MAX;

        return min + rand() % (max - min); // NOLINT(cert-msc50-cpp)
    }

    float Game::randFloat(float min, float max) {
        if (min == max)
            return min;

        if (min > max)
            std::swap(min, max);

        if (max > RAND_MAX)
            max = RAND_MAX;

        return min + static_cast<float>(rand()) // NOLINT(cert-msc50-cpp)
                / (static_cast<float> (RAND_MAX / (max - min)));
    }

}
