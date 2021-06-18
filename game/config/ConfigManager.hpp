#pragma once


#include "PhysicsConfig.hpp"
#include "GameConfig.hpp"

namespace awd::game {

    class ConfigManager {
    private:
        bool loadGameConfig   ();
        bool loadPhysicsConfig();

    public:
        GameConfig    game;
        PhysicsConfig physics;

        bool loadConfigs();
    };

}

