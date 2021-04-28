#include "Screen.hpp"

namespace awd::game {

    Screen::Screen(ScreenType type, std::shared_ptr<sf::RenderWindow>& window) {
        this->type = type;
        this->window = window;
    }

    ScreenType Screen::getType() const {
        return type;
    }

    std::shared_ptr<sf::RenderWindow> Screen::getWindow() const {
        return window;
    }

    void Screen::render() {
        renderComponents();
    }

    void Screen::renderComponents() {
        for (const auto& comp : components)
            comp->render();
    }

}
