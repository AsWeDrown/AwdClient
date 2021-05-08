#pragma once


#include "Button.hpp"
#include "RichText.hpp"

namespace awd::game {

    class TextButton : public Button {
    private:
        unsigned int hoverTicks = 0;

        std::unique_ptr<sf::RectangleShape> line       = nullptr,
                                            fill       = nullptr;

        std::unique_ptr<sf::Text>           buttonText = nullptr;


    public:
        TextButton(id_type id,
                   float renderScale,
                   const std::shared_ptr<sf::RenderWindow>& window,
                   const std::wstring& text,
                   float x, float y,
                   float width, float height,
                   const std::shared_ptr<ButtonListener>& listener);

        void update() override;
        void draw() override;
    };

}
