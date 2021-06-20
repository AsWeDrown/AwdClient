#pragma once


#include "SolidHandler.hpp"

namespace awd::game {

    class FinalCaptainDoorHandler : public SolidHandler {
    public:
        bool canInteract(const Entity& entity) override;
    };

}

