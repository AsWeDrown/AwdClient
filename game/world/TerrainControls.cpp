#include <cmath>
#include <iostream>
#include "TerrainControls.hpp"
#include "../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    TerrainControls::TerrainControls(const std::shared_ptr<WorldData>& worldData) {
        this->worldData = worldData;
    }

    TileBlock& TerrainControls::getTileAt(uint32_t posX, uint32_t posY) const {
        return *shareTileAt(posX, posY);
    }

    std::shared_ptr<TileBlock> TerrainControls::shareTileAt(uint32_t posX, uint32_t posY) const {
        for (const auto& tile : worldData->tiles)
            if (tile->posX == posX && tile->posY == posY)
                return tile;

        throw std::invalid_argument("tile position out of world range: (" + std::to_string(posX) +
                                    ", " + std::to_string(posY) + "); expected " +
                                    "x in range [0; " + std::to_string(worldData->width) + "), " +
                                    "y in range [0; " + std::to_string(worldData->height) + ")");
    }

    void TerrainControls::replaceTileAt(uint32_t posX, uint32_t posY, uint32_t newTileId) {
        /* Заменяем данные */

        TileBlock tile = getTileAt(posX, posY);
        tile.tileId = newTileId;
        tile.handler = TileData::newTileHandler(newTileId);

        /* Заменяем текстуру // скопировано с WorldLoader */

        // Размеры файла с текстурами всех тайлов (tilemap).
        uint32_t wholeTilemapWidth  = Game::instance().getTextures()->worldTileMap->getSize().x;

        // Ищем позицию этого тайла в tilemap (в "таблице текстур" тайлов).
        uint32_t tileX = newTileId % (wholeTilemapWidth / worldData->tileSize);
        uint32_t tileY = newTileId / (wholeTilemapWidth / worldData->tileSize);

        // Указатель на Quad (4 точки) этого тайла.
        sf::Vertex* quad = &(*worldData->worldVertices)[4 * (posX + posY * worldData->width)];

        // Задаём текстуры 4 точек этого Quad'а.
        // (Имеются в виду координаты нужной текстуры в tilemap (в "таблцие текстур" тайлов) - используем tileSize.)
        quad[0].texCoords = sf::Vector2f(
                tileX       * worldData->tileSize,  // NOLINT(cppcoreguidelines-narrowing-conversions)
                tileY       * worldData->tileSize); // NOLINT(cppcoreguidelines-narrowing-conversions)
        quad[1].texCoords = sf::Vector2f(
                (tileX + 1) * worldData->tileSize,  // NOLINT(cppcoreguidelines-narrowing-conversions)
                tileY       * worldData->tileSize); // NOLINT(cppcoreguidelines-narrowing-conversions)
        quad[2].texCoords = sf::Vector2f(
                (tileX + 1) * worldData->tileSize,  // NOLINT(cppcoreguidelines-narrowing-conversions)
                (tileY + 1) * worldData->tileSize); // NOLINT(cppcoreguidelines-narrowing-conversions)
        quad[3].texCoords = sf::Vector2f(
                tileX       * worldData->tileSize,  // NOLINT(cppcoreguidelines-narrowing-conversions)
                (tileY + 1) * worldData->tileSize); // NOLINT(cppcoreguidelines-narrowing-conversions)
    }

    float TerrainControls::advanceTowardsXUntilTerrainCollision(const Entity& entity, float destWorldX) const {
        float entityX = entity.getPosX();
        float dx = destWorldX - entityX;

        if (dx == 0.0f)
            return entityX;

        bool pathingRight = dx > 0.0f;

        BoundingBox entityBb = entity.getBoundingBox();
        BoundingBox destEntityBb = entityBb.deepCopy().move(dx, 0.0f);

        int leftmostTileX   = (int) std::floor(destEntityBb.getMinX());
        int rightmostTileX  = (int) std::ceil (entityBb    .getMaxX());
        int topmostTileY    = (int) std::floor(destEntityBb.getMinY());
        int bottommostTileY = (int) std::ceil (entityBb    .getMaxY());

        int tileX = pathingRight ? rightmostTileX : leftmostTileX;

        for (int tileY = topmostTileY; tileY < bottommostTileY; tileY++) {
            TileBlock nearbyTile = getTileAt(tileX, tileY);
            BoundingBox nearbyTileBb = nearbyTile.getBoundingBox();

            if (!nearbyTile.handler->isPassableBy(entity) && destEntityBb.intersectsWith(nearbyTileBb))
                return pathingRight
                       ? entityX + (nearbyTileBb.getMinX() - entityBb.getMaxX())
                       : entityX - (entityBb.getMinX() - nearbyTileBb.getMaxX());
        }

        return destWorldX;
    }

    float TerrainControls::advanceTowardsYUntilTerrainCollision(const Entity& entity, float destWorldY) const {
        float entityY = entity.getPosY();
        float dy = destWorldY - entityY;

        if (dy == 0.0f)
            return entityY;

        BoundingBox entityBb = entity.getBoundingBox();
        BoundingBox destEntityBb = entityBb.deepCopy().move(0.0f, dy);

        int leftmostTileX   = (int) std::floor(destEntityBb.getMinX());
        int rightmostTileX  = (int) std::ceil (entityBb    .getMaxX());
        int topmostTileY    = (int) std::floor(destEntityBb.getMinY());
        int bottommostTileY = (int) std::ceil (entityBb    .getMaxY());

        bool pathingBottom = dy > 0.0f;
        int tileY = pathingBottom ? bottommostTileY : topmostTileY;

        for (int tileX = leftmostTileX; tileX < rightmostTileX; tileX++) {
            TileBlock nearbyTile = getTileAt(tileX, tileY);
            BoundingBox nearbyTileBb = nearbyTile.getBoundingBox();

            if (!nearbyTile.handler->isPassableBy(entity) && destEntityBb.intersectsWith(nearbyTileBb)) {
                return pathingBottom
                       ? entityY + (nearbyTileBb.getMinY() - entityBb.getMaxY())
                       : entityY - (entityBb.getMinY() - nearbyTileBb.getMaxY());
            }
        }

        return destWorldY;
    }

    bool TerrainControls::isOnGround(const Entity& entity) const {
        BoundingBox entityBb = entity.getBoundingBox();

        float massCenterX = entityBb.getCenterX();
        float entityFeetY = entityBb.getMaxY   ();

        if (std::fmod(entityFeetY, 1.0f) != 0.0f)
            return false;

        int leftmostTileX   = (int) std::floor(massCenterX);
        int rightmostTileX  = (int) std::ceil (massCenterX);
        int tileY           = (int) entityFeetY;

        TileBlock tilesBeneath[] = {
                getTileAt(leftmostTileX,  tileY),
                getTileAt(rightmostTileX, tileY)
        };

        for (const auto& tileBeneath : tilesBeneath) {
            if (!tileBeneath.handler->isPassableBy(entity)) {
                BoundingBox tileBeneathBb = tileBeneath.getBoundingBox();

                if (entityBb.isAboveOf(tileBeneathBb)
                        && entityBb.isCenterHorizontallyWithinOf(tileBeneathBb))
                    return true;
            }
        }

        return false;
    }

    std::optional<TileBlock> TerrainControls::getFirstIntersectingTile(const Entity& entity,
                                                                       const std::function<bool(const TileBlock&)>& pred) {
        BoundingBox entityBb = entity.getBoundingBox();

        int leftmostTileX   = (int) std::floor(entityBb.getMinX());
        int rightmostTileX  = (int) std::ceil (entityBb.getMaxX());
        int topmostTileY    = (int) std::floor(entityBb.getMinY());
        int bottommostTileY = (int) std::ceil (entityBb.getMaxY());

        for (int tileX = leftmostTileX; tileX < rightmostTileX; tileX++) {
            for (int tileY = topmostTileY; tileY < bottommostTileY; tileY++) {
                TileBlock nearbyTile = getTileAt(tileX, tileY);
                BoundingBox nearbyTileBb = nearbyTile.getBoundingBox();

                if (pred(nearbyTile) && entityBb.intersectsWith(nearbyTileBb))
                    return nearbyTile;
            }
        }

        return {}; // null (not found)
    }

}
