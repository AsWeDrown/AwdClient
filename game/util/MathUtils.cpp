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

    std::string MathUtils::pseudoRound(float num, uint32_t places) {
        std::string s = std::to_string(num);
        bool floatingPointMet = false;
        uint32_t placesAppended = 0;
        std::string result;

        for (char c : s) {
            result += c;

            if (c == '.')
                floatingPointMet = true;
            else if (floatingPointMet && ++placesAppended == places)
                break;
        }

        return result;
    }
    
}
