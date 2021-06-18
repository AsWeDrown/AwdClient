// Токены парсера level meta.
#define TOK_WORLD_WIDTH "WorldWidth"
#define TOK_WORLD_HEIGHT "WorldHeight"
#define TOK_TILE_SIZE "TileSize"

// Подробнее - см. WorldData#displayTileSize.
#define DISPLAY_TILE_SIZE 64


#include <fstream>
#include <iostream>
#include "WorldLoader.hpp"
#include "../util/ImageUtils.hpp"
#include "../Game.hpp"
#include "TileData.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void WorldLoader::processToken(const std::string& token, WorldData& targetWorldData) {
        if (token == TOK_WORLD_WIDTH) {
            if (parseState == LevelParseState::EXPECTING_IDENTIFIER)
                parseState = LevelParseState::EXPECTING_WORLD_WIDTH_INT_VAL;
            else {
                std::wcerr << L"Invalid token in level data file: unexpected identifier" << std::endl;
                loadStatus = WorldLoadStatus::PARSE_ERROR;
            }
        } else if (token == TOK_WORLD_HEIGHT) {
            if (parseState == LevelParseState::EXPECTING_IDENTIFIER)
                parseState = LevelParseState::EXPECTING_WORLD_HEIGHT_INT_VAL;
            else {
                std::wcerr << L"Invalid token in level data file: unexpected identifier" << std::endl;
                loadStatus = WorldLoadStatus::PARSE_ERROR;
            }
        } else if (token == TOK_TILE_SIZE) {
            if (parseState == LevelParseState::EXPECTING_IDENTIFIER)
                parseState = LevelParseState::EXPECTING_TILE_SIZE_INT_VAL;
            else {
                std::wcerr << L"Invalid token in level data file: unexpected identifier" << std::endl;
                loadStatus = WorldLoadStatus::PARSE_ERROR;
            }
        } else {
            try {
                int intVal = std::stoi(token);

                if (intVal <= 0)
                    throw std::invalid_argument("non-positive integers are not allowed");

                switch (parseState) {
                    case LevelParseState::EXPECTING_WORLD_WIDTH_INT_VAL:
                        targetWorldData.width = intVal;
                        break;

                    case LevelParseState::EXPECTING_WORLD_HEIGHT_INT_VAL:
                        targetWorldData.height = intVal;
                        break;

                    case LevelParseState::EXPECTING_TILE_SIZE_INT_VAL:
                        targetWorldData.tileSize = intVal;
                        break;

                    default:
                        std::wcerr << L"Invalid token in level data file: expected identifier" << std::endl;
                        loadStatus = WorldLoadStatus::PARSE_ERROR;

                        break;
                }

                parseState = LevelParseState::EXPECTING_IDENTIFIER;
            } catch (const std::invalid_argument&) {
                std::wcerr << L"Invalid token in level data file: expected positive 32-bit integer" << std::endl;
                loadStatus = WorldLoadStatus::PARSE_ERROR;
            } catch (const std::out_of_range&) {
                std::wcerr << L"Invalid token in level data file: expected positive 32-bit integer" << std::endl;
                loadStatus = WorldLoadStatus::PARSE_ERROR;
            }
        }
    }

    void WorldLoader::processPixel(uint32_t wholeTilemapWidth, uint32_t x, uint32_t y,
                                   int rgb, WorldData& targetWorldData) {
        // Конвертируем RGB-цвет пикселя в ID тайла (текстуры).
        auto cursor = TileData::rgbToTileIdMap.find(rgb);

        if (cursor == TileData::rgbToTileIdMap.cend()) {
            std::wcerr << L"Invalid tile RGB #" << std::hex << rgb
                       << L" at (" << x << L", " << y << L")" << std::endl;
            loadStatus = WorldLoadStatus::BITMAP_ERROR;

            return;
        }

        uint32_t tileId = cursor->second;

        // Сохраняем базовую информацию об этом тайле в памяти.
        auto tileBlock = std::make_shared<TileBlock>();

        tileBlock->tileId = tileId;
        tileBlock->posX   = x;
        tileBlock->posY   = y;
        tileBlock->handler = TileData::newTileHandler(tileId);

        targetWorldData.tiles.push_back(tileBlock);

        // Ищем позицию этого тайла в tilemap (в "таблице текстур" тайлов).
        uint32_t tileX = tileId % (wholeTilemapWidth / targetWorldData.tileSize);
        uint32_t tileY = tileId / (wholeTilemapWidth / targetWorldData.tileSize);

        // Указатель на Quad (4 точки) этого тайла.
        sf::Vertex* quad = &(*targetWorldData.worldVertices)[4 * (x + y * targetWorldData.width)];

        // Задаём координаты 4 точек этого Quad'а.
        // Для размещения точек используем отмасштабированный под текущий дисплей размер тайлов (displayTileSize).
        quad[0].position = sf::Vector2f(
                x       * targetWorldData.displayTileSize,      // NOLINT(cppcoreguidelines-narrowing-conversions)
                y       * targetWorldData.displayTileSize);     // NOLINT(cppcoreguidelines-narrowing-conversions)
        quad[1].position = sf::Vector2f(
                (x + 1) * targetWorldData.displayTileSize,      // NOLINT(cppcoreguidelines-narrowing-conversions)
                y       * targetWorldData.displayTileSize);     // NOLINT(cppcoreguidelines-narrowing-conversions)
        quad[2].position = sf::Vector2f(
                (x + 1) * targetWorldData.displayTileSize,      // NOLINT(cppcoreguidelines-narrowing-conversions)
                (y + 1) * targetWorldData.displayTileSize);     // NOLINT(cppcoreguidelines-narrowing-conversions)
        quad[3].position = sf::Vector2f(
                x       * targetWorldData.displayTileSize,      // NOLINT(cppcoreguidelines-narrowing-conversions)
                (y + 1) * targetWorldData.displayTileSize);     // NOLINT(cppcoreguidelines-narrowing-conversions)

        // Задаём текстуры 4 точек этого Quad'а.
        // (Имеются в виду координаты нужной текстуры в tilemap (в "таблцие текстур" тайлов) - используем tileSize.)
        quad[0].texCoords = sf::Vector2f(
                tileX       * targetWorldData.tileSize,  // NOLINT(cppcoreguidelines-narrowing-conversions)
                tileY       * targetWorldData.tileSize); // NOLINT(cppcoreguidelines-narrowing-conversions)
        quad[1].texCoords = sf::Vector2f(
                (tileX + 1) * targetWorldData.tileSize,  // NOLINT(cppcoreguidelines-narrowing-conversions)
                tileY       * targetWorldData.tileSize); // NOLINT(cppcoreguidelines-narrowing-conversions)
        quad[2].texCoords = sf::Vector2f(
                (tileX + 1) * targetWorldData.tileSize,  // NOLINT(cppcoreguidelines-narrowing-conversions)
                (tileY + 1) * targetWorldData.tileSize); // NOLINT(cppcoreguidelines-narrowing-conversions)
        quad[3].texCoords = sf::Vector2f(
                tileX       * targetWorldData.tileSize,  // NOLINT(cppcoreguidelines-narrowing-conversions)
                (tileY + 1) * targetWorldData.tileSize); // NOLINT(cppcoreguidelines-narrowing-conversions)
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    WorldLoader::WorldLoader(uint32_t dimension) {
        this->dimension = dimension;
    }

    void WorldLoader::operator >>(WorldData& targetWorldData) {
        if (loadStatus != WorldLoadStatus::NOT_LOADED)
            throw std::runtime_error("loadWorld() called twice");

        std::wcout << L"Loading dimension " << dimension << std::endl;
        targetWorldData.dimension = dimension;

        try {
            // Читаем и обрабатываем метаданные о мире (размеры текстур и т.п.).
            std::string dimFolder = "assets/worlds/dim_" + std::to_string(dimension);
            std::ifstream levelMeta(dimFolder + "/level-meta.dat");
            std::string token;

            while (levelMeta >> token) {
                processToken(token, targetWorldData);

                if (loadStatus == WorldLoadStatus::PARSE_ERROR) {
                    std::wcerr << L"World Loader: Parse error" << std::endl;
                    return;
                }
            }

            if (targetWorldData.width == 0 || targetWorldData.height == 0 || targetWorldData.tileSize == 0) {
                std::wcerr << L"World Loader: File error: reached EOF but not fully parsed yet" << std::endl;
                loadStatus = WorldLoadStatus::FILE_ERROR;

                return;
            }

            // Размеры файла с текстурами всех тайлов (tilemap).
            uint32_t wholeTilemapWidth  = Game::instance().getTextures()->worldTileMap->getSize().x;
            uint32_t wholeTilemapHeight = Game::instance().getTextures()->worldTileMap->getSize().y;

            if ((wholeTilemapWidth % targetWorldData.tileSize) != 0
                    || (wholeTilemapHeight % targetWorldData.tileSize) != 0) {
                std::wcerr << L"World loader: Compat error: level-meta specifies tile size of "
                           << targetWorldData.tileSize << L"x" << targetWorldData.tileSize
                           << L", but either the width or the height of the tilemap.png texture "
                           << L"(" << wholeTilemapWidth << L"x" << wholeTilemapHeight << L") "
                           << L"is not a multiple of " << targetWorldData.tileSize << std::endl;

                loadStatus = WorldLoadStatus::COMPAT_ERROR;

                return;
            }

            targetWorldData.displayTileSize = DISPLAY_TILE_SIZE * Game::instance().getRenderScale();

            std::wcout << L"World size: " << targetWorldData.width           << L"x"
                                          << targetWorldData.height          << L" x "
                                          << targetWorldData.tileSize        << L"x"
                                          << targetWorldData.tileSize        << L" ("
                                          << targetWorldData.displayTileSize << L"x"
                                          << targetWorldData.displayTileSize
                                          << L" pixels per tile on screen)"  << std::endl;

            // Читаем и обрабатываем "начинку" мира (местоположение тайлов и т.п.).
            std::shared_ptr<Pixels> scheme = ImageUtils::
                    readBitmapImage((dimFolder + "/level-scheme.bmp").c_str());

            if (scheme->bmpWidth != targetWorldData.width
                    || scheme->bmpHeight != targetWorldData.height) {
                std::wcerr << L"World loader: Compat error: level-meta specifies world of size "
                           << targetWorldData.width << L"x" << targetWorldData.height
                           << L", but the size of the level-scheme BMP is "
                           << scheme->bmpWidth << L"x" << scheme->bmpHeight << std::endl;

                loadStatus = WorldLoadStatus::COMPAT_ERROR;

                return;
            }

            targetWorldData.worldVertices = std::make_shared<sf::VertexArray>();
            targetWorldData.worldVertices->setPrimitiveType(sf::Quads);
            targetWorldData.worldVertices->resize(4 * targetWorldData.width * targetWorldData.height);

            for (int y = 0; y < scheme->bmpHeight; y++) {
                for (int x = 0; x < scheme->bmpWidth; x++) {
                    int rgb = scheme->rgbAt(x, y);
                    processPixel(wholeTilemapWidth, x, y, rgb, targetWorldData);

                    if (loadStatus == WorldLoadStatus::BITMAP_ERROR) {
                        std::wcerr << L"World Loader: Bitmap error" << std::endl;
                        return;
                    }
                }
            }

            // Мир загружен успешно.
            loadStatus = WorldLoadStatus::LOADED;
        } catch (const std::ifstream::failure& ex) {
            std::wcerr << L"World Loader: File error: " << ex.what() << std::endl;
            loadStatus = WorldLoadStatus::FILE_ERROR;
        }
    }

    WorldLoadStatus WorldLoader::getLoadStatus() const {
        return loadStatus;
    }

}
