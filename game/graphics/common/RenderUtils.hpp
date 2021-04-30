#pragma once


#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

namespace awd::game {

    class RenderUtils {
    public:
        static void verticalGradientSquare(const std::shared_ptr<sf::RenderWindow>& window,
                                           float startX, float startY, float endX, float endY,
                                           const sf::Color& topColor, const sf::Color& bottomColor);
    };

}
