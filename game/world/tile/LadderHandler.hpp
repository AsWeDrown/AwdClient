#pragma once


#include "VoidHandler.hpp"

namespace awd::game {

    class LadderHandler : public VoidHandler {
    public:
        bool isClimbableBy(const Entity& entity) override;
    };

}

