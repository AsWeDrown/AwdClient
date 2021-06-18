#pragma once


#include <SFML/Graphics.hpp>

namespace awd::game {

    class UniformGridBuilder {
    private:
        float startX, endX, endY, cellsWidth, cellsHeight, currX, currY;
        bool offsetOnRowChange;
        bool fail = false;

        sf::Vector2f buildHere();

    public:
        UniformGridBuilder(float startX, float startY,
                           float endX, float endY,
                           float cellsWidth, float cellsHeight,
                           bool offsetOnRowChange);

        void operator >>(sf::Vector2f& targetVec);
        bool failed() const;
    };

}
