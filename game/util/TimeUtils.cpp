#include "TimeUtils.hpp"
#include <chrono>

namespace awd::game {

    uint64_t TimeUtils::currentTimeMillis() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }

}
