#pragma once


#include "InteractableHandler.hpp"

namespace awd::game {

    class SwitchHandler : public InteractableHandler {
    public:
        bool isPassableBy(const Entity& entity) override;
    };

}

