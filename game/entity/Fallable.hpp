#pragma once


#include <cstdint>

namespace awd::game {

    class Fallable {
    public:
        virtual uint32_t getMidairTicks() const = 0;

        virtual float getFallDistance() const = 0;

        bool isOnGround() const;
    };

}

