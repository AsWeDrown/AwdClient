#include "UniformGridBuilder.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    sf::Vector2f UniformGridBuilder::buildHere() {
        sf::Vector2f builtCell(currX, currY);
        currX += cellsWidth;
        return builtCell;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    UniformGridBuilder::UniformGridBuilder(float startX, float startY,
                                           float endX, float endY,
                                           float cellsWidth, float cellsHeight,
                                           bool offsetOnRowChange) {
        this->currX = this->startX = startX;
        this->currY                = startY;
        this->endX = endX;
        this->endY = endY;
        this->cellsWidth = cellsWidth;
        this->cellsHeight = cellsHeight;
        this->offsetOnRowChange = offsetOnRowChange;
    }

    void UniformGridBuilder::operator >>(sf::Vector2f& targetVec) {
        sf::Vector2f cell(-1.0f, -1.0f); // (-1,-1) возвращается только в случае ошибки

        if (!fail) {
            if (currX + cellsWidth <= endX)
                // Справа есть место для новой ячейки. Строим на месте.
                cell = buildHere();
            else if (currY + cellsHeight <= endY) {
                // Справа нет места для новой ячейки, но снизу ещё есть свободный ряд.
                // Переходим на новый ряд (снизу), ...
                float offsetX = offsetOnRowChange
                        // ... смещая X так, чтобы получилась "кирпичная стена" (максимальное заполнение пространства).
                        ? endX - currX
                        // ... сбрасывая X в начало ("правильная" сетка, заполняющая фиксированную часть пространства).
                        : 0.0f;

                currX = startX + offsetX;
                currY += cellsHeight;
                cell = buildHere();
            } else
                // Для новых ячеек нет места. Совсем.
                fail = true;
        }

        targetVec.x = cell.x;
        targetVec.y = cell.y;
    }

    bool UniformGridBuilder::failed() const {
        return fail;
    }
    
}
