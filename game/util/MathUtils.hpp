#pragma once


#include <string>

namespace awd::game {

    class MathUtils {
    public:
        static float clampAngle360(float degrees);

        static float clampAngle180(float degrees);

        static std::string pseudoRound(float num, uint32_t places);
    };

}

