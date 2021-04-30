#include "RenderUtils.hpp"

namespace awd::game {

    void RenderUtils::verticalGradientSquare(const std::shared_ptr<sf::RenderWindow>& window,
                                             float startX, float startY, float endX, float endY,
                                             const sf::Color& topColor, const sf::Color& bottomColor) {
        sf::Vertex gradient[] = { // координаты точек прямоугольника в порядке по часовой стрелке
                sf::Vertex(sf::Vector2f(startX, startY), topColor),
                sf::Vertex(sf::Vector2f(endX, startY), topColor),
                sf::Vertex(sf::Vector2f(endX, endY), bottomColor),
                sf::Vertex(sf::Vector2f(startX, endY), bottomColor),
        };

        window->draw(gradient, 4, sf::Quads);
    }

}
