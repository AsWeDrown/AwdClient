#include "ScreenComponent.hpp"

namespace awd::game {

    ScreenComponent::ScreenComponent(ScreenComponentType type,
                                     std::shared_ptr<sf::RenderWindow>& window) {
        this->type = type;
        this->window = window;
    }

    ScreenComponentType ScreenComponent::getType() const {
        return type;
    }

}
