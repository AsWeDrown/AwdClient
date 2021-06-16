#pragma once


#include "VoidHandler.hpp"

namespace awd::game {

    class InteractableHandler : public TileHandler {
    public:
        bool isPassableBy(const Entity& entity) override = 0;
        bool canInteract(const Entity& entity) override;
    };

}

