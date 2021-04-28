#include "MainMenuBackground.hpp"

namespace awd::game {

    MainMenuBackground::MainMenuBackground(std::shared_ptr<sf::RenderWindow>& window)
            : ScreenComponent(ScreenComponentType::BACKGROUND, window) {}

    void MainMenuBackground::render() {
        sf::RectangleShape bg(sf::Vector2f(window->getSize().x, window->getSize().y));
        bg.setFillColor(sf::Color::Blue);
        window->draw(bg);
    }

}
