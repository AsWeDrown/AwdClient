#include "TileData.hpp"
#include "tile/VoidHandler.hpp"
#include "tile/SolidHandler.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void TileData::reg(int rgb, uint32_t tileId, const std::shared_ptr<TileHandler>& handler) {
        auto c1 = rgbToTileIdMap.find(rgb   );
        auto c2 = tileHandlers  .find(tileId);

        if (c1 != rgbToTileIdMap.end() || c2 != tileHandlers.end())
            throw std::invalid_argument(
                    "duplicate tile data: rgb=#" + std::to_string(rgb) + ", id=" + std::to_string(tileId));

        rgbToTileIdMap[rgb   ] = tileId ;
        tileHandlers  [tileId] = handler;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    std::map<int,      uint32_t                    > TileData::rgbToTileIdMap;
    std::map<uint32_t, std::shared_ptr<TileHandler>> TileData::tileHandlers  ;

    void TileData::init() {
        // Пустота (void) (тайлы, сквозь которые всегда можно спокойно, беспрепятственно проходить).
        auto voidHandler = std::make_shared<VoidHandler>();
        reg(0xffffff, 0, voidHandler);

        // Твёрдые тайлы (solid).
        auto solidHandler = std::make_shared<SolidHandler>();
        reg(0x000000, 1, solidHandler);
        reg(0x4a4a4a, 2, solidHandler);
        reg(0x636363, 3, solidHandler);
        reg(0x7e7e7e, 4, solidHandler);
        reg(0x9a9a9a, 5, solidHandler);
        reg(0xb5b5b5, 6, solidHandler);
    }

    TileHandler& TileData::getTileHandler(uint32_t tileId) {
        return *tileHandlers[tileId];
    }

}
