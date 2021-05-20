#pragma once


#include <string>

namespace awd::game {

    class GameConfig {
    public:
        void load();

        std::string rootServerHost;
        uint32_t    rootServerPort;
        bool        vsync;
        uint32_t    framerateLimit;
    };

}

