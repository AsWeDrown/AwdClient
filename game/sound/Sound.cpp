#include "Sound.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Sound::Sound(uint32_t id) { // Global
        this->id = id;
        this->sourceX = GLOBAL_SOURCE;
        this->sourceY = GLOBAL_SOURCE;
    }

    Sound::Sound(uint32_t id, float sourceX, float sourceY) {
        this->id = id;
        this->sourceX = sourceX;
        this->sourceY = sourceY;
    }

    uint32_t Sound::getId() const {
        return id;
    }

    float Sound::getSourceX() const {
        return sourceX;
    }

    float Sound::getSourceY() const {
        return sourceY;
    }

    bool Sound::isGlobal() const { // Global
        return sourceX == GLOBAL_SOURCE && sourceY == GLOBAL_SOURCE;
    }

}
