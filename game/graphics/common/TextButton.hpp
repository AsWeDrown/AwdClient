#pragma once


#include "../common/Button.hpp"

namespace awd::game {

    class TextButton : public Button {
    private:
        uint32_t hoverTicks = 0;

        std::unique_ptr<sf::RectangleShape> outline    = nullptr,
                                            fill       = nullptr;

        std::unique_ptr<sf::Text>           buttonText = nullptr;


    public:
        TextButton(id_type id,
                   const std::wstring& text,
                   float x, float y,
                   float width, float height,
                   uint32_t fontSize,
                   const std::shared_ptr<ButtonListener>& listener);

        void update() override;
        void draw() override;
    };

}
