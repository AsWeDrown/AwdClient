#include "LobbyScreen.hpp"
#include "../common/WaterBackground.hpp"
#include "../../Game.hpp"

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
        x      = 0.0f;
        y      = 0.0f;
        width  = window->getSize().x;
        height = window->getSize().y;

        // Фон
        addChild(std::make_shared<WaterBackground>(
                ID_SCREEN_LOBBY_WATER_BACKGROUND, renderScale, window));

        auto lobby = Game::instance().getCurrentLobby();
        //todo

        // Кнопки
        createButtons();
    }

    void LobbyScreen::update() {
        Screen::update();
    }

    void LobbyScreen::draw() {
        Screen::draw();
    }

}
