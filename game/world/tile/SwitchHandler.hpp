#pragma once


#include "VoidHandler.hpp"

namespace awd::game {

    class SwitchHandler : public VoidHandler {
    public:
        bool canInteract(const Entity& entity) override;
    };

}

