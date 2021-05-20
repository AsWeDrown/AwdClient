#include "PhysicsConfig.hpp"
#include "DumbYamlReader.hpp"

namespace awd::game {

    void PhysicsConfig::load() {
        DumbYamlReader yamlReader("assets/configs/physics.yml");
        schemaVersion = yamlReader.getUnsignedInt32("schemaVersion");

        if (schemaVersion != SCHEMA_VERSION)
            throw std::runtime_error("incompatible physics config schema - code: "
                    + std::to_string(SCHEMA_VERSION) + ", file: " + std::to_string(schemaVersion));

        playerBaseHorizontalMoveSpeed = yamlReader.getFloat("playerBaseHorizontalMoveSpeed");
    }

}
