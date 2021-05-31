#include "TileBlock.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    TileHandler& TileBlock::getHandler() const {
        return TileData::getTileHandler(tileId);
    }

    BoundingBox TileBlock::getBoundingBox() const {
        return BoundingBox(posX, posY, posX + 1.0f, posY + 1.0f);
    }

}
