#define HOVERED_TILE_OUTLINE_THICKNESS 2.0f


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

        // TODO WaterBackground? (не работает.. просто не рисуется (фон остаётся чёрным))
    }

    void World::keyPressed(const sf::Event::KeyEvent& event) {
        Drawable::keyPressed(event);
    }

    void World::mousePressed(const sf::Event::MouseButtonEvent& event) {
        Drawable::mousePressed(event);
    }

    void World::update() {
        Drawable::update();

        // Обводка (подсветка) тайлов при наведении на них мышью (если с этим тайлом можно взаимодействовать).
        updateHoveredTile();
    }

    void World::draw() {
        // Сначала рисуем сам мир (карту) ...
        if (worldData != nullptr) {
            sf::RenderStates states(Game::instance().getTextures()->worldTileMap.get());
            window->draw(*worldData->worldVertices, states);
        }

        // ... а затем уже всё остальное.
        Drawable::draw();

        window->draw(*hoveredTileOutline);
    }

    void World::updateHoveredTile() {
        float dispTileSizePixels = worldData->displayTileSize; // NOLINT(cppcoreguidelines-narrowing-conversions)

        sf::Vector2i mouseScreenPos = sf::Mouse::getPosition(*window);
        sf::Vector2f mouseWorldPos = window->mapPixelToCoords(mouseScreenPos);
        sf::Vector2i mouseTilePos = sf::Vector2i(
                static_cast<int>(mouseWorldPos.x / dispTileSizePixels),
                static_cast<int>(mouseWorldPos.y / dispTileSizePixels)
        );

        uint32_t tilePosX = mouseTilePos.x;
        uint32_t tilePosY = mouseTilePos.y;

        try {
            hoveredTile = terrainControls->shareTileAt(tilePosX, tilePosY);

            // FIXME: Не костыль, а костылище! (Пользуемся тем, что на клиенте canInteract не зависит от типа Entity.)
            if (!entities.empty() && hoveredTile->handler->canInteract(*entities[0])) {
                // С тайлом, на котороый наведён курсор, можно взаимодействовать. Выделяем его.
                hoveredTileOutline->setOutlineColor(sf::Color::Yellow);
                hoveredTileOutline->setPosition(
                        dispTileSizePixels * hoveredTile->posX,  // NOLINT(cppcoreguidelines-narrowing-conversions)
                        dispTileSizePixels * hoveredTile->posY // NOLINT(cppcoreguidelines-narrowing-conversions)
                );
            } else
                // С тайлом, на который наведён курсор, нельзя взаимодействовать. Снимаем выделение (если оно было).
                hoveredTileOutline->setOutlineColor(sf::Color::Transparent);
        } catch (const std::invalid_argument& ex) {
            // Курсор за границами мира?
            hoveredTile = nullptr;
        }
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

            // Измерение загружено успешно. Обновляем данные.
            float outlineThickness = HOVERED_TILE_OUTLINE_THICKNESS * renderScale + 1.0f; // min 1 px
            float dispTileSizePixels = worldData->displayTileSize; // NOLINT(cppcoreguidelines-narrowing-conversions)
            float outlineRectSize = outlineThickness + dispTileSizePixels;
            hoveredTileOutline = std::make_unique<sf::RectangleShape>(
                    sf::Vector2f(outlineRectSize, outlineRectSize));
            hoveredTileOutline->setFillColor(sf::Color::Transparent);
            hoveredTileOutline->setOutlineThickness(outlineThickness);

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

    std::shared_ptr<TileBlock> World::getHoveredTile() const {
        return hoveredTile;
    }

}
