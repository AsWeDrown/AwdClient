#pragma once


#include <memory>
#include <SFML/Graphics.hpp>

class shared_ptr;
namespace awd::game {

    struct RectWrapper {
        float preservedWidth, preservedHeight;
        std::shared_ptr<sf::RectangleShape> rect;
    };

}

