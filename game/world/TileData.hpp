#pragma once


#include <cstdint>
#include <map>
#include "tile/TileHandler.hpp"

namespace awd::game {

    class TileData {
    private:
        static void reg(int rgb, uint32_t tileId, const std::shared_ptr<TileHandler>& handler);

    public:
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //   Для конверсии RGB-цвета пикселей из level-scheme в ID (номера) тайлов (текстур).
        //   https://github.com/AsWeDrown/awd-protocol/wiki/%D0%9D%D0%B0%D1%87%D0%B8%D0%BD%D0%BA%D0%B0-%D0%BC%D0%B8%D1%80%D0%B0-(World-Contents)
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static std::map<int, uint32_t> rgbToTileIdMap;

        static std::map<uint32_t, std::shared_ptr<TileHandler>> tileHandlers;

        static void init();

        static TileHandler& getTileHandler(uint32_t tileId);
    };

}

