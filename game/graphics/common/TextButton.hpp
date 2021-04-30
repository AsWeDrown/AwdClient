#pragma once


#include "Button.hpp"

namespace awd::game {

    class TextButton : public Button {
    private:
        std::wstring text;
        void (*buttonClicked)(Drawable* buttonOwner);
        unsigned int hoverTicks = 0;

    public:
        TextButton(float renderScale,
                   const std::shared_ptr<sf::RenderWindow>& window,
                   const std::wstring& text,
                   unsigned int x, unsigned int y,
                   unsigned int width, unsigned int height,
                   void (*clickAction)(Drawable* buttonOwner));

        void onLeftClick() override;
        void update() override;
        void draw() override;
    };

}

