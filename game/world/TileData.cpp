#include "TileData.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void TileData::reg(int rgb, uint32_t tileId, const std::shared_ptr<TileHandlerFactory>& factory) {
        auto c1 = rgbToTileIdMap.find(rgb);
        auto c2 = tileHandlerFactories.find(tileId);

        if (c1 != rgbToTileIdMap.end() || c2 != tileHandlerFactories.end())
            throw std::invalid_argument(
                    "duplicate tile data: rgb=" + std::to_string(rgb) +
                    " (decimal), id=" + std::to_string(tileId));

        tileHandlerFactories[tileId] = factory;
        rgbToTileIdMap[rgb] = tileId;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    std::map<int,      uint32_t                           > TileData::rgbToTileIdMap      ;
    std::map<uint32_t, std::shared_ptr<TileHandlerFactory>> TileData::tileHandlerFactories;

    void TileData::init() {
        // Пустота (void) (тайлы, сквозь которые всегда можно спокойно, беспрепятственно проходить).
        auto voidHandlerFactory = std::make_shared<VoidHandlerFactory>();
        reg(0xffffff,   0, voidHandlerFactory);
        reg(0x84659c,   8, voidHandlerFactory);
        reg(0x9a78b4,   9, voidHandlerFactory);
        reg(0xacb4e2,  10, voidHandlerFactory);
        reg(0x5e5e5e,  11, voidHandlerFactory);
        reg(0xd24a44,  12, voidHandlerFactory);

        // Твёрдые тайлы (solid).
        auto solidHandlerFactory = std::make_shared<SolidHandlerFactory>();
        reg(0x000000,   1, solidHandlerFactory);
        reg(0x4a4a4a,   2, solidHandlerFactory);
        reg(0x636363,   3, solidHandlerFactory);
        reg(0x7e7e7e,   4, solidHandlerFactory);
        reg(0x9a9a9a,   5, solidHandlerFactory);
        reg(0xb5b5b5,   6, solidHandlerFactory);

        // Лестницы.
        auto ladderHandlerFactory = std::make_shared<LadderHandlerFactory>();
        reg(0x865e3a,   7, ladderHandlerFactory);

        // Тайлы, с которыми можно взаимодействовать.
        auto switchHandlerFactory = std::make_shared<SwitchHandlerFactory>();
        reg(0x9ac561,  13, switchHandlerFactory);
        reg(0x6e8a47,  14, switchHandlerFactory);
    }

    std::shared_ptr<TileHandler> TileData::newTileHandler(uint32_t tileId) {
        return tileHandlerFactories[tileId]->newTileHandler();
    }

}
