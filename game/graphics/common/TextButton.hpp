#pragma once


#include "Button.hpp"

namespace awd::game {

    class TextButton : public Button {
    private:
        std::wstring text;
        unsigned int hoverTicks = 0;

    public:
        TextButton(id_type id,
                   float renderScale,
                   const std::shared_ptr<sf::RenderWindow>& window,
                   const std::wstring& text,
                   unsigned int x, unsigned int y,
                   unsigned int width, unsigned int height,
                   const std::shared_ptr<ButtonListener>& listener);

        void update() override;
        void draw() override;
    };

}
