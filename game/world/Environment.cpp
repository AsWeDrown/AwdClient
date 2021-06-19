#include "Environment.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void Environment::update(uint32_t envBitfield) {
        envEnableAlarm = (envBitfield & ENABLE_ALARM) != 0;
    }

    bool Environment::enableAlarm() const {
        return envEnableAlarm;
    }

}
