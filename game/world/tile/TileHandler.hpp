#pragma once


#include "../../entity/Entity.hpp"

namespace awd::game {

    class TileHandler {
    public:
        virtual bool isPassableBy(const Entity& entity) = 0;

        virtual bool isClimbableBy(const Entity& entity);
    };

}

