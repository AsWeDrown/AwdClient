#pragma once


#include "TileHandler.hpp"

namespace awd::game {

    class VoidHandler : public TileHandler {
    public:
        bool isPassableBy(const Entity& entity) override;
    };

}

