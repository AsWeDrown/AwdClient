#include "LobbyScreen.hpp"
#include "../common/WaterBackground.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void LobbyScreen::createButtons() {

    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    LobbyScreen::LobbyScreen(float renderScale,
                             const std::shared_ptr<sf::RenderWindow>& window)
                             : Screen(ID_SCREEN_LOBBY, renderScale, window) {
        x = 0;
        y = 0;
        width = window->getSize().x;
        height = window->getSize().y;

        addChild(std::make_shared<WaterBackground>(
                ID_SCREEN_LOBBY_WATER_BACKGROUND, renderScale, window));

        createButtons();
    }

    void LobbyScreen::update() {
        Screen::update();
    }

    void LobbyScreen::draw() {
        Screen::draw();
    }

}
