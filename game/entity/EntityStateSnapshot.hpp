#pragma once


#include <SFML/Graphics.hpp>

namespace awd::game {

    class EntityStateSnapshot {
    public:
        uint32_t localSequence = 0;

        float posX      = 0.0f,
              posY      = 0.0f,
              faceAngle = 0.0f;
    };

}
