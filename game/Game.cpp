// Проверка ОС (мы поддерживаем только Windows).
#if !defined(__CYGWIN__) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__))
    #define IS_RUNNING_WINDOWS true
#else
    #define IS_RUNNING_WINDOWS false
#endif
// Основные параметры.
#define CONNECTION_TIMEOUT_MILLIS 10000
#define GAME_TPS (int64_t) 25L
#define BASE_SCREEN_WIDTH 1920.0f
#define BASE_SCREEN_HEIGHT 1080.0f
// Число наносекунд в одной секунде.
#define NANOS_IN_SECOND (int64_t) 1E+9L
// Макрос для менее громоздкой регистрации PacketListener'ов.
#define PLISTENER(x)  packetManager->registerListener( \
                              net::PacketWrapper::PacketCase::k##x, \
                              std::make_shared<x##Listener>() \
                      );


#include <thread>
#include "Game.hpp"
#include "packetlistener/auth/HandshakeResponseListener.hpp"
#include "packetlistener/lobby/CreateLobbyResponseListener.hpp"
#include "graphics/mainmenu/MainMenuScreen.hpp"
#include "packetlistener/auth/PingListener.hpp"
#include "packetlistener/lobby/LeaveLobbyResponseListener.hpp"
#include "packetlistener/lobby/JoinLobbyResponseListener.hpp"
#include "packetlistener/lobby/UpdatedMembersListListener.hpp"
#include "packetlistener/lobby/KickedFromLobbyListener.hpp"
#include "graphics/play/PlayScreen.hpp"
#include "packetlistener/lobby/BeginPlayStateResponseListener.hpp"
#include "packetlistener/play/UpdateDimensionCommandListener.hpp"
#include "packetlistener/play/JoinWorldCommandListener.hpp"
#include "packetlistener/play/SpawnEntityListener.hpp"
#include "packetlistener/play/UpdateEntityPositionListener.hpp"
#include "packetlistener/play/DespawnEntityListener.hpp"
#include "packetlistener/play/BeginQuestListener.hpp"
#include "packetlistener/play/AdvanceQuestListener.hpp"
#include "packetlistener/play/EndQuestListener.hpp"
#include "packetlistener/play/UpdateTileListener.hpp"
#include "packetlistener/play/DisplayChatMessageListener.hpp"
#include "packetlistener/play/UpdateEnvironmentListener.hpp"
#include "util/CrashReporter.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool Game::loadAssets() {
        std::wcout << L"--- Loading assets ---" << std::endl;

        return configs ->loadConfigs ()
            && fonts   ->loadFonts   ()
            && textures->loadTextures();
    }

    void Game::registerPacketListeners() {
        std::wcout << L"Client protocol version: " << net::PacketManager::PROTOCOL_VERSION << std::endl;

        PLISTENER(HandshakeResponse)
        PLISTENER(Ping)
        PLISTENER(CreateLobbyResponse)
        PLISTENER(JoinLobbyResponse)
        PLISTENER(LeaveLobbyResponse)
        PLISTENER(UpdatedMembersList)
        PLISTENER(KickedFromLobby)
        PLISTENER(BeginPlayStateResponse)
        PLISTENER(UpdateDimensionCommand)
        PLISTENER(JoinWorldCommand)
        PLISTENER(SpawnEntity)
        PLISTENER(UpdateEntityPosition)
        PLISTENER(DespawnEntity)
        PLISTENER(BeginQuest)
        PLISTENER(AdvanceQuest)
        PLISTENER(EndQuest)
        PLISTENER(UpdateTile)
        PLISTENER(DisplayChatMessage)
        PLISTENER(UpdateEnvironment)
    }

    void Game::startGameLoop() {
        // Инициализация базовых полей, зависящих от конфига/настроек (не могут быть инициализированы в конструкторе).
        udpClient = std::make_shared<net::UdpClient>(
                configs->game.rootServerHost, configs->game.rootServerPort);

        packetManager = std::make_shared<net::PacketManager >(udpClient    );
        netService    = std::make_shared<net::NetworkService>(packetManager);

        // Появления игры на экране.
        auto bestVideoMode = sf::VideoMode::getFullscreenModes()[2];

        window = std::make_shared<sf::RenderWindow>(
                bestVideoMode, "As We Drown"/*, sf::Style::Fullscreen*/);

        if (configs->game.vsync)
            window->setVerticalSyncEnabled(true);
        else
            window->setFramerateLimit(configs->game.framerateLimit);

        renderScale = std::min((float) bestVideoMode.width  / BASE_SCREEN_WIDTH,
                               (float) bestVideoMode.height / BASE_SCREEN_HEIGHT);

        std::wcout << L"Video mode: " << bestVideoMode.width << L"x" << bestVideoMode.height
                  << L" (" << bestVideoMode.bitsPerPixel << L" bits per pixel)" << std::endl;
        std::wcout << L"Render scale: " << renderScale << std::endl;

        currentState = GameState::AUTH;
        currentScreen = std::make_shared<MainMenuScreen>();
        tickDelayNanos = std::chrono::nanoseconds(NANOS_IN_SECOND / GAME_TPS).count();
        lastFrameNanoTime = std::make_shared<game_time>(game_clock::now());

        try {
            while (window->isOpen() && currentState != GameState::EXIT)
                runGameLoop();
        } catch (const std::exception& ex) {
            CrashReporter::makeReport("Unhandled exception in main game loop", ex);
        }

        std::wcout << L"Reached end of game loop." << std::endl;
        shutdown(); // на случай, если выход был произведён, скажем, по крестику, а не через кнопку в главном меню
    }

    void Game::runGameLoop() {
        // Засекаем время начала обновления в самом начале.
        auto thisFrameNanoTime = std::make_shared<game_time>(game_clock::now());

        // События (нажатия клавиш и пр.).
        sf::Event event; // NOLINT(cppcoreguidelines-pro-type-member-init)

        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window->close();
                    break;

                case sf::Event::LostFocus:
                    gameWindowFocused = false;
                    break;

                case sf::Event::GainedFocus:
                    gameWindowFocused = true;
                    break;

                case sf::Event::KeyPressed:
                    if (gameWindowFocused)
                        currentScreen->keyPressed(event.key);

                    break;

                case sf::Event::MouseButtonPressed:
                    if (gameWindowFocused)
                        currentScreen->mousePressed(event.mouseButton);

                    break;
            }
        }

        // Обновления состояния игры (игровые тики).
        //
        // Такой подход (вместо, например, sleep или простого измерения времени с последнего тика)
        // важен для соблюдения максимально возможного постоянства TPS (при других подходах он будет
        // колебаться от 23-24 до 25-26, что совершенно неприемлемо). В Java это же (или максимально
        // близкое к нему) поведение реализует Timer#scheduleAtFixedRate, здесь же реализуем сами:
        // https://gafferongames.com/post/fix_your_timestep/
        int64_t nanosSinceLastTick = std::chrono::duration_cast
                <std::chrono::nanoseconds>(*thisFrameNanoTime - *lastFrameNanoTime).count();

        lastFrameNanoTime      = thisFrameNanoTime;
        frameAccumulatorNanos += nanosSinceLastTick;

        while (frameAccumulatorNanos >= tickDelayNanos) {
            update();
            frameAccumulatorNanos -= tickDelayNanos;
        }

        // Прорисовка.
        window->clear();
        render();
        window->display();
    }

    void Game::update() {
        if (++currentTick == 1)
            // Элементы загрузки, которые выполняются после отображения главного меню (например, соединение).
            postScreenLoad();

        tpsMeter->onUpdate();

        netService->flushReceiveQueue(); // обрабатываем пакеты, полученные с сервера
        currentScreen->update();         // выполняем обновление (м.б., на отправку будут поставлены какие-то пакеты)
        netService->flushSendQueue();    // отправляем пакеты, поставленые в очередь на отправку после обновления
    }

    void Game::render() {
        fpsMeter->onDraw();
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

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Game::Game() {
        tpsMeter      = std::make_shared<TpsMeter>           (GAME_TPS);
        fpsMeter      = std::make_shared<FpsMeter>           ();
    }

    Game::~Game() = default;

    void Game::bootstrap() {
        // Мы работаем только с Windows.
        if (!IS_RUNNING_WINDOWS)
            throw std::runtime_error("unsupported platform (required: Windows)");

        // Инициализация ГПСЧ.
        srand(static_cast<uint32_t>(time(nullptr))); // NOLINT(cert-msc51-cpp)

        // Загрузка текстур, звуков, шрифтов и прочего.
        if (!loadAssets())
            throw std::runtime_error("failed to load assets");

        TileData::init();

        // Главный цикл игры (в этом потоке). Должен запускаться в самом конце!
        startGameLoop();
    }

    void Game::shutdown() {
        if (shuttingDown)
            return; // игнорируем возможные повторные вызовы

        shuttingDown = true;

        std::wcout << L"Shutting down..." << std::endl;

        if (udpClient != nullptr)
            udpClient->tearDown();

        currentState = GameState::EXIT;

        std::wcout << L"Bye!" << std::endl;
    }

    void Game::handshakeComplete(uint32_t serverProtocolVersion) {
        if (serverProtocolVersion == net::PacketManager::PROTOCOL_VERSION) {
            // Процесс аутентификации/рукопожатия прошёл успешно.
            currentState = GameState::LOBBY;
            currentScreen->hideCurrentLoadingOverlay();
        } else {
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

    std::shared_ptr<TpsMeter> Game::getTpsMeter() const {
        return tpsMeter;
    }

    std::shared_ptr<FpsMeter> Game::getFpsMeter() const {
        return fpsMeter;
    }

    std::shared_ptr<ConfigManager> Game::getConfigs() const {
        return configs;
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

    bool Game::isGameFocused() const {
        return gameWindowFocused;
    }

    void Game::setLastRtt(uint32_t rtt) {
        this->lastRtt = rtt;
    }

    uint32_t Game::getCurrentState() const {
        return currentState;
    }

    void Game::setCurrentState(uint32_t newState) {
        this->currentState = newState;
    }

    float Game::getRenderScale() const {
        return renderScale;
    }

    std::shared_ptr<sf::RenderWindow> Game::getWindow() const {
        return window;
    }

    std::shared_ptr<Screen> Game::getCurrentScreen() const {
        return currentScreen;
    }

    void Game::setCurrentScreen(const std::shared_ptr<Screen>& screen) {
        this->currentScreen = screen;
    }

    std::shared_ptr<World> Game::currentWorld() const {
        if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen))
            return playScreen->getWorld();
        else
            return nullptr;
    }

    std::shared_ptr<Lobby> Game::getCurrentLobby() const {
        return currentLobby;
    }

    void Game::setCurrentLobby(const std::shared_ptr<Lobby>& lobby) {
        this->currentLobby = lobby;
    }

    bool Game::isJoinedWorld() const {
        return joinedWorld;
    }

    void Game::setJoinedWorld(bool joined) {
        this->joinedWorld = joined;
    }

    void Game::returnToMainMenu() {
        std::wcout << L"Returning to the main menu" << std::endl;

        // Возвращаемся на главный экран.
        currentState = GameState::LOBBY;
        currentScreen = std::make_shared<MainMenuScreen>();
        window->setView(window->getDefaultView()); // сброс View

        // Сбрасываем всё, что связано с комнатой, в которой мы только что играли.
        currentLobby = nullptr;
        netService->leaveLobbyRequest(); // уведомляем всех о том, что мы вышли из комнаты
        joinedWorld = false;
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
