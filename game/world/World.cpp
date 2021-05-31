#include "World.hpp"
#include "../Game.hpp"
#include "WorldLoader.hpp"
#include "../util/CrashReporter.hpp"
#include "../entity/Fallable.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    World::World() : Drawable(ID_SCREEN_PLAY_WORLD) {
        x      = 0.0f;
        y      = 0.0f;
        width  = window->getSize().x; // NOLINT(cppcoreguidelines-narrowing-conversions)
        height = window->getSize().y; // NOLINT(cppcoreguidelines-narrowing-conversions)
    }

    void World::keyPressed(const sf::Event::KeyEvent& event) {
        Drawable::keyPressed(event);
    }

    void World::mousePressed(const sf::Event::MouseButtonEvent& event) {
        Drawable::mousePressed(event);
    }

    void World::update() {
        Drawable::update();
    }

    void World::draw() {
        // Сначала рисуем сам мир (карту) ...
        if (worldData != nullptr) {
            sf::RenderStates states(Game::instance().getTextures()->worldTileMap.get());
            window->draw(*worldData->worldVertices, states);
        }

        // ... а затем уже всё остальное.
        Drawable::draw();
    }

    std::shared_ptr<WorldData> World::getWorldData() const {
        return worldData;
    }

    void World::focusCamera(float worldX, float worldY) {
        // Размеры отображаемой части мира (фокуса) (View) - в тайлах.
        float viewWidth           = WORLD_SIZE_ON_SCREEN_PART * worldData->width; // NOLINT(cppcoreguidelines-narrowing-conversions)
        float viewHeight          = WORLD_SIZE_ON_SCREEN_PART * worldData->height; // NOLINT(cppcoreguidelines-narrowing-conversions)

        // Половины размеров отображаемой части мира (фокуса) (View) - в тайлах.
        float halfViewWidthTiles  = viewWidth  / 2.0f; // NOLINT(cppcoreguidelines-narrowing-conversions)
        float halfViewHeightTiles = viewHeight / 2.0f; // NOLINT(cppcoreguidelines-narrowing-conversions)

        // Накладываем ограничения на расположение X центра фокуса в мире (все рассчёте ведём в тайлах).
        float focusMinWorldX      = halfViewWidthTiles;
        float focusMaxWorldX      = worldData->width - halfViewWidthTiles; // NOLINT(cppcoreguidelines-narrowing-conversions)
        float focusWorldX         = std::min(focusMaxWorldX, std::max(focusMinWorldX, worldX));

        // Накладываем ограничения на расположение Y центра фокуса в мире (все рассчёте ведём в тайлах).
        float focusMinWorldY      = halfViewHeightTiles;
        float focusMaxWorldY      = worldData->height - halfViewHeightTiles; // NOLINT(cppcoreguidelines-narrowing-conversions)
        float focusWorldY         = std::min(focusMaxWorldY, std::max(focusMinWorldY, worldY));

        // Переводим координаты фокуса из игровой системы координат (тайлы) в пиксели.
        float focusWorldXPixel    = focusWorldX * worldData->displayTileSize; // NOLINT(cppcoreguidelines-narrowing-conversions)
        float focusWorldYPixel    = focusWorldY * worldData->displayTileSize; // NOLINT(cppcoreguidelines-narrowing-conversions)
        float viewWidthPixels     = viewWidth   * worldData->displayTileSize; // NOLINT(cppcoreguidelines-narrowing-conversions)
        float viewHeightPixels    = viewHeight  * worldData->displayTileSize; // NOLINT(cppcoreguidelines-narrowing-conversions)

        window->setView(sf::View(
                sf::Vector2f(focusWorldXPixel, focusWorldYPixel),
                sf::Vector2f(viewWidthPixels, viewHeightPixels)
        ));
    }

    void World::centerCamera() {
        focusCamera(
                worldData->width  / 2.0f, // NOLINT(cppcoreguidelines-narrowing-conversions
                worldData->height / 2.0f  // NOLINT(cppcoreguidelines-narrowing-conversions
        );
    }

    void World::updateDimension(uint32_t newDimension) {
        if (this->dimension != newDimension) {
            // Загружаем новое измерение.
            this->dimension = newDimension;

            WorldLoader worldLoader(dimension);
            worldData = std::make_shared<WorldData>();
            worldLoader >> *worldData;

            if (worldLoader.getLoadStatus() != WorldLoadStatus::LOADED) {
                // Скорее всего, файлы игры были повреждены.
                worldData = nullptr;
                CrashReporter::makeReport(
                        "Failed to update dimension (world load error): " + std::to_string(newDimension));
                Game::instance().shutdown();

                return;
            }

            terrainControls = std::make_shared<TerrainControls>(worldData);
            std::wcout << L"Updated dimension successfully" << std::endl;

            // Изначально фокусируем камеру по центру мира.
            centerCamera();
        }
    }

    std::shared_ptr<Entity> World::getEntityById(uint32_t entityId) const {
        for (const auto& entity : entities)
            if (entity->getEntityId() == entityId)
                return entity;

        return nullptr;
    }

    void World::addEntity(const std::shared_ptr<Entity>& entity) {
        if (getChildById(entity->getId()) != nullptr) {
            std::wcerr << L"addEntity() called twice for entity " << entity->getEntityId()
                       << L" (Drawable ID: " << entity->getId() << L")" << std::endl;

            return;
        }

        enqueueAddChild(entity);
        entities.push_back(entity);
    }

    void World::removeEntity(uint32_t entityId) {
        entities.erase(std::remove_if(
                entities.begin(), entities.end(),
                [entityId](const std::shared_ptr<Entity>& entity) {
                    return entity->getEntityId() == entityId;
                }), entities.end()
        );

        enqueueRemoveChild(Entity::entityIdToDrawableId(entityId));
    }

    std::shared_ptr<TerrainControls> World::getTerrainControls() const {
        return terrainControls;
    }

}
