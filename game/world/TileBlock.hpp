#pragma once


#include "TileData.hpp"

namespace awd::game {

    class TileBlock : public Collidable {
    public:
        uint32_t tileId = 0, // ID текстуры этого тайла в tilemap
                 posX   = 0, // координата X левого верхнего угла этого тайла в мире
                 posY   = 0; // координата Y левого верхнего угла этого тайла в мире

        TileHandler& getHandler() const;

        BoundingBox getBoundingBox() const override;
    };

}

