#pragma once


#include "../graphics/common/Drawable.hpp"
#include "WorldData.hpp"
#include "../entity/Entity.hpp"
#include "TerrainControls.hpp"

namespace awd::game {

    class World : public Drawable {
    private:
        uint32_t dimension = 9999; // 9999 -> ничего не рисуем (мир ещё не загружен)

        std::shared_ptr<WorldData> worldData = nullptr;

        std::shared_ptr<TerrainControls> terrainControls = nullptr;

        std::vector<std::shared_ptr<Entity>> entities;

        std::shared_ptr<TileBlock> hoveredTile = nullptr;

        std::unique_ptr<sf::RectangleShape> hoveredTileOutline = nullptr;

    public:
        /**
         * Часть мира (%/100), отображаемая на экрана (находящаяся в фокусе, т.е. в текущем View).
         */
        static constexpr float WORLD_SIZE_ON_SCREEN_PART = 0.225f; // 22.5%

        World();

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        void updateHoveredTile();

        std::shared_ptr<WorldData      > getWorldData      () const;
        std::shared_ptr<TerrainControls> getTerrainControls() const;
        std::shared_ptr<TileBlock      > getHoveredTile    () const;

        void focusCamera(float worldX, float worldY);
        void centerCamera();

        void updateDimension(uint32_t newDimension);

        std::shared_ptr<Entity> getEntityById(uint32_t entityId) const;
        void                    addEntity    (const std::shared_ptr<Entity>& entity);
        void                    removeEntity (uint32_t entityId);
    };

}

