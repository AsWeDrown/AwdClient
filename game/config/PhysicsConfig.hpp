#pragma once


/**
 * TODO - менять каждый раз при добавлении новых полей или удалении старых
 *        (чтобы было предупреждение о необходимости обновить файл).
 */
#define SCHEMA_VERSION 1


#include <string>

namespace awd::game {

    class PhysicsConfig {
    public:
        void load();

        uint32_t schemaVersion;

        float playerBaseHorizontalMoveSpeed;
    };

}

