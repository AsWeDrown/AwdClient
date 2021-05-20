#include "GameConfig.hpp"
#include "DumbYamlReader.hpp"


namespace awd::game {

    void GameConfig::load() {
        DumbYamlReader yamlReader("assets/configs/game.yml");

        rootServerHost = yamlReader.getString       ("rootServerHost");
        rootServerPort = yamlReader.getUnsignedInt32("rootServerPort");
        vsync          = yamlReader.getBoolean      ("vsync"         );
        framerateLimit = yamlReader.getUnsignedInt32("framerateLimit");
    }

}
