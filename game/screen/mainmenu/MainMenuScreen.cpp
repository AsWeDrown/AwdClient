#include <SFML/Graphics/RectangleShape.hpp>
#include "MainMenuScreen.hpp"
#include "MainMenuBackground.hpp"

namespace awd::game {

    MainMenuScreen::MainMenuScreen(std::shared_ptr<sf::RenderWindow>& window)
            : Screen(ScreenType::MAIN_MENU, window) {
        components.emplace_back(std::make_shared<MainMenuBackground>(window));
        //todo buttons etc
    }

}
