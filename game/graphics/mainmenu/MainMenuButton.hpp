#pragma once


#include "../Button.hpp"

namespace awd::game {

    class MainMenuButton : public Button {
    private:
        std::wstring text;
        void (*clickAction)();
        unsigned int hoverTicks = 0;

    public:
        MainMenuButton(float renderScale,
                       const std::shared_ptr<sf::RenderWindow>& window,
                       const std::wstring& text,
                       unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height,
                       void (*clickAction)());

        void onLeftClick() override;
        void update() override;
        void draw() override;
    };

}

