#pragma once


#include <memory>
#include <functional>
#include "WorldData.hpp"

namespace awd::game {

    class TerrainControls {
    private:
        std::shared_ptr<WorldData> worldData;

    public:
        explicit TerrainControls(const std::shared_ptr<WorldData>& worldData);

        TileBlock& getTileAt(uint32_t posX, uint32_t posY) const;

        /**
         * Вычисляет значение X, в которое может переместиться указанная сущность в этом мире,
         * максимально близкое к указанному (желаемому). Для этого учитывается "начинка" мира
         * (окружающие сущность тайлы и прочее), а также размеры и текущая позиция сущности.
         *
         * @param entity сущность, которая хочет передвинуться по X.
         * @param destWorldX желаемая координата X в мире.
         *
         * @return координата X в мире, в которую указанная сущность может (имеет право) передвинуться
         *         с учётом различных законов физики и взаимодействий, наиболее близкая к желаемому X.
         */
        float advanceTowardsXUntilTerrainCollision(const Entity& entity, float destWorldX) const;

        /**
         * Вычисляет значение Y, в которое может переместиться указанная сущность в этом мире,
         * максимально близкое к указанному (желаемому). Для этого учитывается "начинка" мира
         * (окружающие сущность тайлы и прочее), а также размеры и текущая позиция сущности.
         *
         * @param entity сущность, которая хочет передвинуться по Y.
         * @param destWorldY желаемая координата Y в мире.
         *
         * @return координата Y в мире, в которую указанная сущность может (имеет право) передвинуться
         *         с учётом различных законов физики и взаимодействий, наиболее близкая к желаемому Y.
         */
        float advanceTowardsYUntilTerrainCollision(const Entity& entity, float destWorldX) const;

        /**
         * Проверяет, находится ли указанная сущность сейчас в устойчивом положении на земле.
         * При проверке используется "начинка" мира и местоположение и размеры сущности.
         *
         * @param entity сущность, которую нужно проверить.
         *
         * @return true, если центр масс указанной сущность сейчас устойчив, т.е. сущность не падает
         *         вниз; false - если сущность падает вниз.
         */
        bool isOnGround(const Entity& entity) const;

        std::optional<TileBlock> getFirstIntersectingTile(const Entity& entity,
                                                          const std::function<bool(const TileBlock&)>& pred);
    };

}

