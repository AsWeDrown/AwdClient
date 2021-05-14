#include "PlayScreen.hpp"
#include "../../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    PlayScreen::PlayScreen() : Screen(ID_SCREEN_PLAY) {
        // Собственно, мир (карта).
        world = std::make_shared<World>();
        enqueueAddChild(world);
    }

    void PlayScreen::keyPressed(const sf::Event::KeyEvent& event) {
        Screen::keyPressed(event);
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
    }

    void PlayScreen::draw() {
        Screen::draw();
    }

    std::shared_ptr<PlayScreenListener> PlayScreen::getListener() const {
        return listener;
    }

    std::shared_ptr<World> PlayScreen::getWorld() const {
        return world;
    }

}
