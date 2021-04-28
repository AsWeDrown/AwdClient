#pragma once


#include "../Screen.hpp"

namespace awd::game {

    class MainMenuScreen : public Screen {
    public:
        explicit MainMenuScreen(std::shared_ptr<sf::RenderWindow>& window);
    };

}

