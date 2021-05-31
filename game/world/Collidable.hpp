#pragma once


#include "BoundingBox.hpp"

namespace awd::game {

    /**
     * Любой объект, который имеет какой-то хитбокс (BoundingBox).
     */
    class Collidable {
    public:
        virtual BoundingBox getBoundingBox() const = 0;
    };

}

