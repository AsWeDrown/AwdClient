#pragma once


#include <memory>
#include <SFML/Graphics.hpp>

class shared_ptr;
namespace awd::game {

    class RectWrapper {
    private:
        unsigned int preservedWidth, preservedHeight;
        std::shared_ptr<sf::RectangleShape> rect;

    public:
        RectWrapper(unsigned int preservedWidth, unsigned int preservedHeight,
                    const std::shared_ptr<sf::RectangleShape>& rect);

        unsigned int getPreservedWidth() const;
        unsigned int getPreservedHeight() const;
        std::shared_ptr<sf::RectangleShape> getRect() const;
    };

}

