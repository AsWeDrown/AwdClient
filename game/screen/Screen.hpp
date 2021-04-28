#pragma once


#include <vector>
#include "ScreenComponent.hpp"

namespace awd::game {

    enum class ScreenType {
        MAIN_MENU
    };

    class Screen {
    protected:
        ScreenType type;
        std::shared_ptr<sf::RenderWindow> window = nullptr;
        std::vector<std::shared_ptr<ScreenComponent>> components;

    public:
        Screen(ScreenType type, std::shared_ptr<sf::RenderWindow>& window);

        ScreenType getType() const;
        std::shared_ptr<sf::RenderWindow> getWindow() const;

        virtual void render();
        void renderComponents();
    };

}
