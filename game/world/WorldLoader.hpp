#pragma once


#include <cstdint>
#include "WorldData.hpp"

namespace awd::game {

    enum class LevelParseState {
        EXPECTING_IDENTIFIER,
        EXPECTING_WORLD_WIDTH_INT_VAL,
        EXPECTING_WORLD_HEIGHT_INT_VAL,
        EXPECTING_TILE_SIZE_INT_VAL
    };

    enum class WorldLoadStatus {
        NOT_LOADED,  // загрузка мира ещё на выполнялась (operator >>)
        LOADED,      // мир успешно загружен
        FILE_ERROR,  // не удалось открыть/прочитать какой-либо из файлов, связанных с миром
        PARSE_ERROR, // не удалось обработать файл с данными о мире из-за синтаксической ошибки в формате файла
        BITMAP_ERROR // не удалось обработать файл с "начинкой" мира из-за ошибки в формате файла схемы (BMP)
    };

    class WorldLoader {
    private:
        uint32_t dimension;
        std::map<int, uint32_t> rgbToTileIdMap;
        LevelParseState parseState = LevelParseState::EXPECTING_IDENTIFIER;
        WorldLoadStatus loadStatus = WorldLoadStatus::NOT_LOADED;

        void processToken(const std::string& token, WorldData& targetWorldData);
        void processPixel(uint32_t x, uint32_t y, int rgb, WorldData& targetWorldData);

    public:
        explicit WorldLoader(uint32_t dimension);

        void operator >>(WorldData& targetWorldData);
        WorldLoadStatus getLoadStatus() const;
    };

}

