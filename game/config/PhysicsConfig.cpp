#include "PhysicsConfig.hpp"
#include "DumbYamlReader.hpp"

namespace awd::game {

    void PhysicsConfig::load() {
        DumbYamlReader yamlReader("assets/configs/physics.yml");
        schemaVersion = yamlReader.getUnsignedInt32("schemaVersion");

        if (schemaVersion != SCHEMA_VERSION)
            throw std::runtime_error("incompatible physics config schema - code: "
                    + std::to_string(SCHEMA_VERSION) + ", file: " + std::to_string(schemaVersion));

        maxLag                     = yamlReader.getUnsignedInt32("maxLag"                    );
        interpDelay                = yamlReader.getUnsignedInt32("interpDelay"               );
        interpBufSizeThreshold     = yamlReader.getUnsignedInt32("interpBufSizeThreshold"    );

        baseEntityPlayerMs         = yamlReader.getFloat        ("baseEntityPlayerMs"        );
        baseEntityPlayerClimbSpeed = yamlReader.getFloat        ("baseEntityPlayerClimbSpeed");
        baseEntityPlayerW          = yamlReader.getFloat        ("baseEntityPlayerW"         );
        baseEntityPlayerH          = yamlReader.getFloat        ("baseEntityPlayerH"         );

        freeFallAcce               = yamlReader.getFloat        ("freeFallAcce"              );
    }

}
