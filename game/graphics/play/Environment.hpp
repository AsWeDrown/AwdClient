#pragma once


#include <cstdint>

namespace awd::game {

    class Environment {
    private:
        static constexpr uint32_t ENABLE_ALARM  = 0b1;

        bool envEnableAlarm;

    public:
        void update(uint32_t envBitfield);

        bool enableAlarm() const;
    };

}

