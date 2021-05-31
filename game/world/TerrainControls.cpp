#include <cmath>
#include <iostream>
#include "TerrainControls.hpp"

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
        for (const auto& tile : worldData->tiles)
            if (tile->posX == posX && tile->posY == posY)
                return *tile;

        throw std::invalid_argument("tile position out of world range: (" + std::to_string(posX) +
                                    ", " + std::to_string(posY) + "); expected " +
                                    "x in range [0; " + std::to_string(worldData->width) + "), " +
                                    "y in range [0; " + std::to_string(worldData->height) + ")");
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
        int rightmostTileX  = (int) std::ceil (destEntityBb.getMaxX());
        int topmostTileY    = (int) std::floor(destEntityBb.getMinY());
        int bottommostTileY = (int) std::ceil (destEntityBb.getMaxY());

        int tileX = pathingRight ? rightmostTileX : leftmostTileX;

        for (int tileY = topmostTileY; tileY < bottommostTileY; tileY++) {
            TileBlock nearbyTile = getTileAt(tileX, tileY);
            BoundingBox nearbyTileBb = nearbyTile.getBoundingBox();

            if (!nearbyTile.getHandler().isPassableBy(entity) && destEntityBb.intersectsWith(nearbyTileBb)) {
                return pathingRight
                       ? entityX + (nearbyTileBb.getMinX() - entityBb.getMaxX())
                       : entityX - (entityBb.getMinX() - nearbyTileBb.getMaxX());
            }
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

        int leftmostTileX   = (int) std::floor(entityBb.getMinX());
        int rightmostTileX  = (int) std::ceil (entityBb.getMaxX());
        int topmostTileY    = (int) std::floor(entityBb.getMinY());
        int bottommostTileY = (int) std::ceil (entityBb.getMaxY());

        bool pathingBottom = dy > 0.0f;
        int tileY = pathingBottom ? bottommostTileY : topmostTileY;

        for (int tileX = leftmostTileX; tileX < rightmostTileX; tileX++) {
            TileBlock nearbyTile = getTileAt(tileX, tileY);
            BoundingBox nearbyTileBb = nearbyTile.getBoundingBox();

            if (!nearbyTile.getHandler().isPassableBy(entity) && destEntityBb.intersectsWith(nearbyTileBb)) {
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
            if (!tileBeneath.getHandler().isPassableBy(entity)) {
                BoundingBox tileBeneathBb = tileBeneath.getBoundingBox();

                if (entityBb.isAboveOf(tileBeneathBb)
                        && entityBb.isCenterHorizontallyWithinOf(tileBeneathBb))
                    return true;
            }
        }

        return false;
    }

}
