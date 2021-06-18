#pragma once


#include <cstdint>

namespace awd::game {

    class Entities {
    public:
        class EntityPlayer {
        public:
            static constexpr uint32_t TYPE = 1;

            static constexpr uint32_t ANIM_BASE_STILL_FRONT = 0; // Стоит на месте. Лицом к пользователю.

            static constexpr uint32_t ANIM_BASE_WALK_RIGHT_0 = 1000; // Идёт вправо (1-й этап анимации).
            static constexpr uint32_t ANIM_BASE_WALK_RIGHT_1 = 1001; // ^ ......... (2-й этап анимации).
            static constexpr uint32_t ANIM_BASE_WALK_RIGHT_2 = 1002; // ^ ......... (3-й этап анимации).
        };
    };

}
