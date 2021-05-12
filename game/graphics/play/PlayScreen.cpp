#include "PlayScreen.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    PlayScreen::PlayScreen() : Screen(ID_SCREEN_PLAY) {}

    void PlayScreen::keyPressed(const sf::Event::KeyEvent& event) {
        Screen::keyPressed(event);
    }

    void PlayScreen::mousePressed(const sf::Event::MouseButtonEvent& event) {
        Screen::mousePressed(event);
    }

    void PlayScreen::update() {
        Screen::update();
    }

    void PlayScreen::draw() {
        Screen::draw();
    }

    std::shared_ptr<PlayScreenListener> PlayScreen::getListener() const {
        return listener;
    }

}
