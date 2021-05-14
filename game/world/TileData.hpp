#pragma once


#include <cstdint>
#include <map>

namespace awd::game {

    class TileData {
    private:
        static std::map<int, uint32_t> createRgbToTileIdMap();

    public:
        static std::map<int, uint32_t> rgbToTileIdMap;
    };

}

