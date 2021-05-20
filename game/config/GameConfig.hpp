#pragma once


/**
 * TODO - менять каждый раз при добавлении новых полей или удалении старых
 *        (чтобы было предупреждение о необходимости обновить файл).
 */
#define SCHEMA_VERSION 1


#include <string>

namespace awd::game {

    class GameConfig {
    public:
        void load();

        uint32_t schemaVersion;

        std::string rootServerHost;
        uint32_t    rootServerPort;
        bool        vsync;
        uint32_t    framerateLimit;
    };

}

