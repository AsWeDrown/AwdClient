#pragma once


#include <cstdint>

namespace awd::game {

    class Sound {
    private:
        static constexpr float GLOBAL_SOURCE = -1.0f; // Global

        uint32_t id;
        float sourceX, sourceY;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        // -- sounds/
        static constexpr uint32_t ROOF_HEAD_HIT  = 1;
        static constexpr uint32_t LOCKER_FALL    = 2;
        static constexpr uint32_t PLAYER_STEP    = 3;
        static constexpr uint32_t SWITCH_TOGGLE  = 4;
        static constexpr uint32_t HATCH_TOGGLE   = 5;
        static constexpr uint32_t BUTTON_CLICK   = 1000000001;
        // -- music/
        static constexpr uint32_t MAIN_MENU_THEME = 1;

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        explicit Sound(uint32_t id); // Global

        Sound(uint32_t id, float sourceX, float sourceY);

        uint32_t getId     () const;
        float    getSourceX() const;
        float    getSourceY() const;
        bool     isGlobal  () const; // Global
    };

}

