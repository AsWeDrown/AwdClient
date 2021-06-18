#include "GameConfig.hpp"
#include "DumbYamlReader.hpp"


namespace awd::game {

    void GameConfig::load() {
        DumbYamlReader yamlReader("assets/configs/game.yml");
        schemaVersion = yamlReader.getUnsignedInt32("schemaVersion");

        if (schemaVersion != SCHEMA_VERSION)
            throw std::runtime_error("incompatible game config schema - code: "
                    + std::to_string(SCHEMA_VERSION) + ", file: " + std::to_string(schemaVersion));

        rootServerHost = yamlReader.getString       ("rootServerHost");
        rootServerPort = yamlReader.getUnsignedInt32("rootServerPort");
        vsync          = yamlReader.getBoolean      ("vsync"         );
        framerateLimit = yamlReader.getUnsignedInt32("framerateLimit");
    }

}
