#include "TileData.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    std::map<int, uint32_t> TileData::createRgbToTileIdMap() {
        ///////////////////////////////////////////////////////////////////////////////////////
        // Для конверсии RGB-цвета пикселей из level-scheme в ID (номера) тайлов (текстур).
        // Подробнее в файле "bmp_pixel_rgb_to_tile_id_mapping.png".
        ///////////////////////////////////////////////////////////////////////////////////////
        std::map<int, uint32_t> map;

        // Пустота (полностью прозрачная текстура).
        map[0xffffff] = 0;

        // Твёрдые (solid) тайлы
        map[0x000000] = 1;
        map[0x4a4a4a] = 2;

        return map;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    std::map<int, uint32_t> TileData::rgbToTileIdMap = createRgbToTileIdMap(); // NOLINT(cert-err58-cpp)

}
