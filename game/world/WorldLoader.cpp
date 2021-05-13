#define TOK_WORLD_WIDTH "WorldWidth"
#define TOK_WORLD_HEIGHT "WorldHeight"
#define TOK_TIlE_SIZE "TileSize"


#include <fstream>
#include <iostream>
#include "WorldLoader.hpp"
#include "../util/ImageUtils.hpp"
#include "../Game.hpp"

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
        } else if (token == TOK_TIlE_SIZE) {
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
            } catch (const std::out_of_range&) {
                std::wcerr << L"Invalid token in level data file: expected positive 32-bit integer" << std::endl;
            }
        }
    }

    void WorldLoader::processPixel(uint32_t x, uint32_t y, int rgb, WorldData& targetWorldData) {
        // Конвертируем RGB-цвет пикселя в ID тайла (текстуры).
        auto cursor = rgbToTileIdMap.find(rgb);

        if (cursor == rgbToTileIdMap.cend()) {
            std::wcerr << L"Invalid tile RGB " << rgb << L" at (" << x << L", " << y << L")" << std::endl;
            loadStatus = WorldLoadStatus::BITMAP_ERROR;

            return;
        }

        uint32_t tileId = cursor->second;

        // Сохраняем базовую информацию об этом тайле в памяти.
        auto tileBlock = std::make_shared<TileBlock>();

        tileBlock->tileId = tileId;
        tileBlock->posX   = x;
        tileBlock->posY   = y;

        targetWorldData.tiles.push_back(tileBlock);

        // Ищем позицию этого тайла в tilemap (в "таблице текстур" тайлов).
        uint32_t tilemapWidth = Game::instance().getTextures()->worldTileMap->getSize().x;
        uint32_t tileX        = tileId % (tilemapWidth / targetWorldData.tileSize);
        uint32_t tileY        = tileId / (tilemapWidth / targetWorldData.tileSize);

        // Указатель на Quad (4 точки) этого тайла.
        sf::Vertex* quad = &(*targetWorldData.worldVertices)[4 * (x + y * targetWorldData.width)];

        // Задаём координаты 4 точек этого Quad'а.
        quad[0].position = sf::Vector2f(
                x       * targetWorldData.tileSize,      // NOLINT(cppcoreguidelines-narrowing-conversions)
                y       * targetWorldData.tileSize);     // NOLINT(cppcoreguidelines-narrowing-conversions)
        quad[1].position = sf::Vector2f(
                (x + 1) * targetWorldData.tileSize,      // NOLINT(cppcoreguidelines-narrowing-conversions)
                y       * targetWorldData.tileSize);     // NOLINT(cppcoreguidelines-narrowing-conversions)
        quad[2].position = sf::Vector2f(
                (x + 1) * targetWorldData.tileSize,      // NOLINT(cppcoreguidelines-narrowing-conversions)
                (y + 1) * targetWorldData.tileSize);     // NOLINT(cppcoreguidelines-narrowing-conversions)
        quad[3].position = sf::Vector2f(
                x       * targetWorldData.tileSize,      // NOLINT(cppcoreguidelines-narrowing-conversions)
                (y + 1) * targetWorldData.tileSize);     // NOLINT(cppcoreguidelines-narrowing-conversions)

        // Задаём текстуры 4 точек этого Quad'а.
        // (Имеются в виду координаты нужной текстуры в tilemap (в "таблцие текстур" тайлов).)
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

        ///////////////////////////////////////////////////////////////////////////////////////
        // Для конверсии RGB-цвета пикселей из level-scheme в ID (номера) тайлов (текстур).
        ///////////////////////////////////////////////////////////////////////////////////////
        rgbToTileIdMap[0xFFFFFF] = 1; // Белый       -- Пустота
        rgbToTileIdMap[0x000000] = 2; // Чёрный      -- Металлический корпус подлодки (без контура)
        rgbToTileIdMap[0x666666] = 3; // Тёмно-серый -- Металлический корпус подлодки (контур сверху)
    }

    void WorldLoader::operator >>(WorldData& targetWorldData) {
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

            std::wcout << L"World size: " << targetWorldData.width    << L"x"
                                          << targetWorldData.height   << L" x "
                                          << targetWorldData.tileSize << L"x"
                                          << targetWorldData.tileSize << std::endl;

            // Читаем и обрабатываем "начинку" мира (местоположение тайлов и т.п.).
            std::shared_ptr<Pixels> scheme = ImageUtils
                    ::readBitmapImage((dimFolder + "/level-scheme.bmp").c_str());

            targetWorldData.worldVertices = std::make_unique<sf::VertexArray>();
            targetWorldData.worldVertices->setPrimitiveType(sf::Quads);
            targetWorldData.worldVertices->resize(4 * targetWorldData.width * targetWorldData.height);

            for (int y = 0; y < scheme->bmpHeight; y++) {
                for (int x = 0; x < scheme->bmpWidth; x++) {
                    int rgb = scheme->rgbAt(x, y);
                    processPixel(x, y, rgb, targetWorldData);

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
