#include "TileBlock.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    BoundingBox TileBlock::getBoundingBox() const {
        return BoundingBox(posX, posY, posX + 1.0f, posY + 1.0f);
    }

}
