#pragma once


/**
 * TODO - менять каждый раз при добавлении новых полей или удалении старых
 *        (чтобы было предупреждение о необходимости обновить файл).
 */
#define SCHEMA_VERSION 1


#include <string>

namespace awd::game {

    /**
     * https://github.com/AsWeDrown/awd-protocol/wiki/Physics-Config
     */
    class PhysicsConfig {
    public:
        void load();

        uint32_t schemaVersion;

        uint32_t maxLag;
        uint32_t interpDelay;
        uint32_t interpBufSizeThreshold;

        float baseEntityPlayerMs;
        float baseEntityPlayerClimbSpeed;
        float baseEntityPlayerW;
        float baseEntityPlayerH;

        float freeFallAcce;
    };

}

