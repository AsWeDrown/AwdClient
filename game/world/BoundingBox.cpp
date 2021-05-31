#include <stdexcept>
#include "BoundingBox.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    BoundingBox::BoundingBox(float minX, float minY, float maxX, float maxY) {
        if (minX < 0.0f || minY < 0.0f || maxX < 0.0f || maxY < 0.0f)
            throw std::invalid_argument(
                    "bounding box coordinates cannot be negative: {"
                    + std::to_string(minX) + ", " + std::to_string(minX) + ", "
                    + std::to_string(maxX) + ", " + std::to_string(maxY) + "}");

        this->minX    = std::min(minX, maxX);
        this->minY    = std::min(minY, maxY);
        this->maxX    = std::max(minX, maxX);
        this->maxY    = std::max(minY, maxY);
        this->centerX = (minX + maxX) / 2.0f;
        this->centerY = (minY + maxY) / 2.0f;
    }

    bool BoundingBox::isHorizontallyWithinOf(const BoundingBox& other) const {
        return minX >= other.minX && maxX <= other.maxX;;
    }

    bool BoundingBox::isVerticallyWithinOf(const BoundingBox& other) const {
        return minY >= other.minY && maxY <= other.maxY;
    }

    bool BoundingBox::isFullyWithinOf(const BoundingBox& other) const {
        return isHorizontallyWithinOf(other) && isVerticallyWithinOf(other);
    }

    bool BoundingBox::isCenterHorizontallyWithinOf(const BoundingBox& other) const {
        return centerX >= other.minX && centerX <= other.maxX;
    }

    bool BoundingBox::isCenterVerticallyWithinOf(const BoundingBox& other) const {
        return centerY >= other.minY && centerY <= other.maxY;
    }

    bool BoundingBox::isCenterFullyWithinOf(const BoundingBox& other) const {
        return isCenterHorizontallyWithinOf(other) && isCenterVerticallyWithinOf(other);
    }

    bool BoundingBox::intersectsWith(const BoundingBox& other) const {
        return minX < other.maxX && maxX > other.minX
            && minY < other.maxY && maxY > other.minY;
    }

    bool BoundingBox::isAboveOf(const BoundingBox& other) const {
        return maxY <= other.minY;
    }

    BoundingBox BoundingBox::deepCopy() const {
        return BoundingBox(minX, minY, maxX, maxY);
    }

    BoundingBox& BoundingBox::move(float deltaX, float deltaY) {
        minX += deltaX;
        maxX += deltaX;
        minY += deltaY;
        maxY += deltaY;

        return *this;
    }

    float BoundingBox::getMinX() const {
        return minX;
    }

    float BoundingBox::getMinY() const {
        return minY;
    }

    float BoundingBox::getMaxX() const {
        return maxX;
    }

    float BoundingBox::getMaxY() const {
        return maxY;
    }

    float BoundingBox::getCenterX() const {
        return centerX;
    }

    float BoundingBox::getCenterY() const {
        return centerY;
    }

}
