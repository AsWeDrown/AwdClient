#pragma once


#include "TileHandler.hpp"

namespace awd::game {

    class LadderHandler : public TileHandler {
    public:
        bool isPassableBy(const Entity& entity) override;

        bool isClimbableBy(const Entity& entity) override;
    };

}

