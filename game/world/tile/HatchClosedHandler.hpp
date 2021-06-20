#pragma once


#include "SolidHandler.hpp"

namespace awd::game {

    class HatchClosedHandler : public SolidHandler {
    public:
        bool canInteract(const Entity& entity) override;
    };

}

