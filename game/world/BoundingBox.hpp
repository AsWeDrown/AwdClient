#pragma once


namespace awd::game {

    class BoundingBox {
    private:
        float minX,    minY,    // левый верхний угол в мире (в тайлах)
              maxX,    maxY,    // правый нижний угол в мире (в тайлах)
              centerX, centerY; // центр в мире (в тайлах)

    public:
        BoundingBox(float minX, float minY, float maxX, float maxY);

        bool isHorizontallyWithinOf(const BoundingBox& other) const;

        bool isVerticallyWithinOf(const BoundingBox& other) const;

        bool isFullyWithinOf(const BoundingBox& other) const;

        bool isCenterHorizontallyWithinOf(const BoundingBox& other) const;

        bool isCenterVerticallyWithinOf(const BoundingBox& other) const;

        bool isCenterFullyWithinOf(const BoundingBox& other) const;

        bool intersectsWith(const BoundingBox& other) const;

        bool isAboveOf(const BoundingBox& other) const;

        BoundingBox deepCopy() const;

        BoundingBox& move(float deltaX, float deltaY);

        float getMinX() const;

        float getMinY() const;

        float getMaxX() const;

        float getMaxY() const;

        float getCenterX() const;

        float getCenterY() const;
    };

}

