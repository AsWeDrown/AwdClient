#include "PhysicsConfig.hpp"
#include "DumbYamlReader.hpp"

namespace awd::game {

    void PhysicsConfig::load() {
        DumbYamlReader yamlReader("assets/configs/physics.yml");

        playerBaseHorizontalMoveSpeed = yamlReader.getFloat("playerBaseHorizontalMoveSpeed");
    }

}
