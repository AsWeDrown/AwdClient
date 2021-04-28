#pragma once


#include "../ScreenComponent.hpp"

namespace awd::game {

    class MainMenuBackground : public ScreenComponent {
    public:
        MainMenuBackground(std::shared_ptr<sf::RenderWindow>& window);

        void render() override;
    };

}

