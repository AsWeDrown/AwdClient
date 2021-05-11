#define HOST "julia.reflex.rip"
#define PORT 23132
#define CONNECTION_TIMEOUT_MILLIS 10000
#define GAME_TPS 25
#define BASE_SCREEN_WIDTH 1920.0f
#define BASE_SCREEN_HEIGHT 1080.0f


#include <thread>
#include "Game.hpp"
#include "packetlistener/HandshakeResponseListener.hpp"
#include "packetlistener/CreateLobbyResponseListener.hpp"
#include "graphics/mainmenu/MainMenuScreen.hpp"
#include "packetlistener/PingListener.hpp"
#include "packetlistener/LeaveLobbyResponseListener.hpp"
#include "packetlistener/JoinLobbyResponseListener.hpp"
#include "packetlistener/UpdatedMembersListListener.hpp"
#include "packetlistener/KickedFromLobbyListener.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool Game::loadAssets() {
        std::wcout << L"--- Loading assets ---" << std::endl;

        return fonts   ->loadFonts()
            && textures->loadTextures();
    }

    void Game::registerPacketListeners() {
        std::wcout << L"Client protocol version: " << net::PacketManager::PROTOCOL_VERSION << std::endl;

        packetManager->registerListener(
                net::PacketWrapper::PacketCase::kHandshakeResponse,
                std::make_shared<HandshakeResponseListener>()
        );

        packetManager->registerListener(
                net::PacketWrapper::PacketCase::kPing,
                std::make_shared<PingListener>()
        );

        packetManager->registerListener(
                net::PacketWrapper::PacketCase::kCreateLobbyResponse,
                std::make_shared<CreateLobbyResponseListener>()
        );

        packetManager->registerListener(
                net::PacketWrapper::PacketCase::kJoinLobbyResponse,
                std::make_shared<JoinLobbyResponseListener>()
        );

        packetManager->registerListener(
                net::PacketWrapper::PacketCase::kLeaveLobbyResponse,
                std::make_shared<LeaveLobbyResponseListener>()
        );

        packetManager->registerListener(
                net::PacketWrapper::PacketCase::kUpdatedMembersList,
                std::make_shared<UpdatedMembersListListener>()
        );

        packetManager->registerListener(
                net::PacketWrapper::PacketCase::kKickedFromLobby,
                std::make_shared<KickedFromLobbyListener>()
        );
    }

    void Game::startGameLoop() {
        auto bestVideoMode = sf::VideoMode::getFullscreenModes()[2];
        window = std::make_shared<sf::RenderWindow>(
                bestVideoMode, "As We Drown"/*, sf::Style::Fullscreen*/);

        float renderScale = std::min((float) bestVideoMode.width  / BASE_SCREEN_WIDTH,
                                     (float) bestVideoMode.height / BASE_SCREEN_HEIGHT);

        std::wcout << L"Video mode: " << bestVideoMode.width << L"x" << bestVideoMode.height
                  << L" (" << bestVideoMode.bitsPerPixel << L" bits per pixel)" << std::endl;
        std::wcout << L"Render scale: " << renderScale << std::endl;

        currentState = GameState::LOBBY;
        currentScreen = std::make_shared<MainMenuScreen>(renderScale, window);

        uint32_t tickDelay = std::chrono::milliseconds(1000 / GAME_TPS).count();
        sf::Clock tickClock;

        while (window->isOpen() && currentState != GameState::EXIT)
            runGameLoop(tickDelay, tickClock);

        std::wcout << L"Reached end of game loop." << std::endl;
        shutdown(); // на случай, если выход был произведён, скажем, по крестику, а не через кнопку в главном меню
    }

    void Game::runGameLoop(uint32_t tickDelay, sf::Clock& tickClock) {
        // События (нажатия клавиш и пр.)
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

        // Обновления (тики)
        if (tickClock.getElapsedTime().asMilliseconds() >= tickDelay) {
            tickClock.restart();
            update();
        }

        // Прорисовка
        window->clear();
        render();
        window->display();
    }

    void Game::update() {
        if (++currentTick == 1)
            postScreenLoad();

        flushPackets();
        currentScreen->update();
    }

    void Game::render() {
        currentScreen->draw();
    }

    void Game::postScreenLoad() {
        // Сообщение загрузки.
        auto mainMenu = std::dynamic_pointer_cast<MainMenuScreen>(currentScreen);
        mainMenu->showLoadingOverlay(L"Подключение...", CONNECTION_TIMEOUT_MILLIS);

        // UDP-клиент (в другом потоке).
        registerPacketListeners();
        udpClient->startInNewThread();
    }

    void Game::flushPackets() {
        netService->flushSendQueue();
        netService->flushReceiveQueue();
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Game::Game() {
        udpClient = std::make_shared<net::UdpClient>(HOST, PORT);
        packetManager = std::make_shared<net::PacketManager>(udpClient);
        netService = std::make_shared<net::NetworkService>(packetManager);
    }

    Game::~Game() = default;

    void Game::bootstrap() {
        // Инициализация ГПСЧ.
        srand(static_cast<uint32_t>(time(nullptr))); // NOLINT(cert-msc51-cpp)

        // Загрузка текстур, звуков, шрифтов и прочего.
        if (!loadAssets()) {
            std::wcerr << L"Failed to load assets." << std::endl;
            return;
        }

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

    void Game::handshakeComplete(uint32_t serverProtocolVersion) {
        if (serverProtocolVersion == net::PacketManager::PROTOCOL_VERSION)
            currentScreen->hideCurrentLoadingOverlay();
        else {
            std::wcerr << L"Protocol versions do not match." << std::endl;

            if (auto mainMenu = std::dynamic_pointer_cast<MainMenuScreen>(currentScreen))
                mainMenu->showErrorDialog(L"{RED}{BOLD}Не удалось подключиться к серверу: "
                                          L"{RESET}{GRAY}версия протокола клиента: {WHITE}"+
                                          std::to_wstring(net::PacketManager::PROTOCOL_VERSION) +
                                          L"{GRAY}, версия протокола сервера: {WHITE}" +
                                          std::to_wstring(serverProtocolVersion) +
                                          L"{GRAY}. Пожалуйста, установите новую версию игры.");
            else
                shutdown();
        }
    }

    void Game::socketBindFailed() {
        std::wcerr << L"Socket bind failed." << std::endl;

        if (auto mainMenu = std::dynamic_pointer_cast<MainMenuScreen>(currentScreen))
            mainMenu->showErrorDialog(L"{RED}{BOLD}Не удалось подключиться к серверу: "
                                      L"{RESET}{GRAY}ошибка привязки сокета. Пожалуйста, убедитесь, "
                                      L"что вы сейчас подключены к Интернету, и что ваш файервол/антивирус не "
                                      L"блокирует использование UDP-порта {WHITE}23132{GRAY}. Также проблема может "
                                      L"быть в том, что в вашей локальной сети уже есть другое устройство, занявшее "
                                      L"этот порт.");
        else
            shutdown();
    }

    uint32_t Game::getCurrentTick() const {
        return currentTick;
    }

    std::shared_ptr<FontManager> Game::getFonts() const {
        return fonts;
    }

    std::shared_ptr<TextureManager> Game::getTextures() const {
        return textures;
    }

    std::shared_ptr<net::PacketManager> Game::getPacketManager() const {
        return packetManager;
    }

    std::shared_ptr<net::NetworkService> Game::getNetService() const {
        return netService;
    }

    uint32_t Game::getLastRtt() const {
        return lastRtt;
    }

    void Game::setLastRtt(uint32_t rtt) {
        this->lastRtt = rtt;
    }

    uint32_t Game::getCurrentState() const {
        return currentState;
    }

    std::shared_ptr<Screen> Game::getCurrentScreen() const {
        return currentScreen;
    }

    void Game::setCurrentScreen(const std::shared_ptr<Screen>& screen) {
        this->currentScreen = screen;
    }

    std::shared_ptr<Lobby> Game::getCurrentLobby() const {
        return currentLobby;
    }

    void Game::setCurrentLobby(const std::shared_ptr<Lobby>& lobby) {
        this->currentLobby = lobby;
    }

    uint32_t Game::randUInt(uint32_t min, uint32_t max) {
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
