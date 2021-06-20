#pragma once


#include "VoidHandler.hpp"

namespace awd::game {

    class HatchOpenHandler : public VoidHandler {
    public:
        bool canInteract(const Entity& entity) override;
    };

}

