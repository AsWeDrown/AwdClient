#include <cmath>
#include "MathUtils.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    float MathUtils::clampAngle360(float degrees) {
        degrees = std::fmod(degrees, 360.0f);
        if (degrees < 0.0f) degrees += 360.0f;
        return degrees;
    }

    float MathUtils::clampAngle180(float degrees) {
        degrees = std::fmod(degrees + 180.0f, 360.0f);
        if (degrees < 0.0f) degrees += 360.0f;
        return degrees - 180.0f;
    }
    
}
