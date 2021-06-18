#pragma once


#include "TileHandler.hpp"

namespace awd::game {

    class SolidHandler : public TileHandler {
    public:
        bool isPassableBy(const Entity& entity) override;
    };

}

