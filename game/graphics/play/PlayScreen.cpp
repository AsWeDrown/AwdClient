// HUD с FPS и пр.
#define PERFORMANCE_HUD_FONT_SIZE 25
#define PERFORMANCE_HUD_MARGIN 17.0f
// Сигнализация
#define MIN_ALARM_RECT_ALPHA 30
#define MAX_ALARM_RECT_ALPHA 3 * MIN_ALARM_RECT_ALPHA
#define ALARM_HALF_CYCLE_TICKS 15
#define ALARM_DELTA_ALPHA (MAX_ALARM_RECT_ALPHA - MIN_ALARM_RECT_ALPHA) / ALARM_HALF_CYCLE_TICKS


#include "PlayScreen.hpp"
#include "../../util/MathUtils.hpp"
#include "../../Game.hpp"
#include "EscPlayDialog.hpp"
#include "../mainmenu/MainMenuScreen.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void PlayScreen::updateEnvironment() {
        updateAlarm();
    }

    void PlayScreen::updateAlarm() {
        if (reducingAlarmAlpha) {
            uint32_t lowerBound = environment->enableAlarm() ? MIN_ALARM_RECT_ALPHA : 0;

            if (currentAlarmAlpha > lowerBound)
                currentAlarmAlpha -= ALARM_DELTA_ALPHA;
            else {
                reducingAlarmAlpha = false;

                if (environment->enableAlarm())
                    Game::instance().getSoundSystem()->playSound(Sound::ALARM_TICK);
            }
        } else if (environment->enableAlarm()) {
            if (currentAlarmAlpha < MAX_ALARM_RECT_ALPHA)
                currentAlarmAlpha += ALARM_DELTA_ALPHA;
            else
                reducingAlarmAlpha = true;
        } else
            reducingAlarmAlpha = true; // сигнализация отключилась в момент загарания (reducingAlarmAlpha=false)

        alarmRect->setFillColor(sf::Color(255, 0, 0, currentAlarmAlpha));

        sf::Vector2f viewCenter = window->getView().getCenter();
        sf::Vector2f viewSize = window->getView().getSize();
        float viewX = viewCenter.x - viewSize.x / 2.0f;
        float viewY = viewCenter.y - viewSize.y / 2.0f;

        alarmRect->setPosition(viewX, viewY);
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    PlayScreen::PlayScreen() : Screen(ID_SCREEN_PLAY) {
        // Собственно, мир (карта).
        world = std::make_shared<World>();
        enqueueAddChild(world);

        // Задания.
        questManager = std::make_shared<QuestManager>();
        enqueueAddChild(questManager);

        // Чат.
        chat = std::make_shared<Chat>();
        enqueueAddChild(chat);

        // Параметры окружения (сам по себе Environments не является Drawable!).
        environment = std::make_shared<Environment>();

        // HUD с FPS, TPS и прочей информацией о производительности и качестве соединения.
        uint32_t perfHudFontSize = PERFORMANCE_HUD_FONT_SIZE * renderScale;

        performanceHud = std::make_unique<sf::Text>();
        performanceHud->setFont(*Game::instance().getFonts()->regularFont);
        performanceHud->setCharacterSize(perfHudFontSize);
        performanceHud->setFillColor(sf::Color::White);

        // Сигнализация.
        alarmRect = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, height));
    }

    void PlayScreen::keyPressed(const sf::Event::KeyEvent& event) {
        Screen::keyPressed(event);

        if (event.code == sf::Keyboard::Escape) {
            // FIXME: всё очень плохо. Ничего из graphics/common не работает нормально при использовании View.
//            if (isDialogOpen())
//                closeCurrentDialog();
//            else
//                openDialog(std::make_shared<EscPlayDialog>(listener, listener, listener));
            Game::instance().returnToMainMenu();
        }
    }

    void PlayScreen::mousePressed(const sf::Event::MouseButtonEvent& event) {
        Screen::mousePressed(event);
    }

    void PlayScreen::update() {
        Screen::update();

        if (justJoined) {
            // Уведомляем сервер о присоединении к миру (для фактического начала игры).
            Game::instance().getNetService()->joinWorldComplete();
            justJoined = false;
        }

        updateEnvironment();

        // HUD с FPS, TPS и прочей информацией о производительности и качестве соединения.
        if (Game::instance().getCurrentTick() % 25 == 0) { // раз в секунду - чтобы не обновлять данные слишком часто
            std::string fps = std::to_string(abs((int)
                    Game::instance().getFpsMeter()->estimateFps()));
            std::string tps = MathUtils::pseudoRound(abs(
                    Game::instance().getTpsMeter()->estimateTps()), 1);
            std::string ping = std::to_string(Game::instance().getLastRtt());
            performanceHud->setString(fps + " FPS | " + tps + " TPS | " + ping + " ms");
        }

        float perfHudMargin = PERFORMANCE_HUD_MARGIN * renderScale;

        sf::Vector2f viewCenter = window->getView().getCenter();
        sf::Vector2f viewSize = window->getView().getSize();
        float viewX = viewCenter.x - viewSize.x / 2.0f;
        float viewY = viewCenter.y - viewSize.y / 2.0f;

        performanceHud->setPosition(viewX + perfHudMargin, viewY + perfHudMargin);
    }

    void PlayScreen::draw() {
        Screen::draw();

        window->draw(*alarmRect);
        window->draw(*performanceHud);
    }

    std::shared_ptr<PlayScreenListener> PlayScreen::getListener() const {
        return listener;
    }

    std::shared_ptr<World> PlayScreen::getWorld() const {
        return world;
    }

    std::shared_ptr<QuestManager> PlayScreen::getQuestManager() const {
        return questManager;
    }

    std::shared_ptr<Chat> PlayScreen::getChat() const {
        return chat;
    }

    std::shared_ptr<Environment> PlayScreen::getEnvironment() const {
        return environment;
    }

}
