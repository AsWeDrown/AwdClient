/**
 * Часть мира (%/100), отображаемая на экрана (находящаяся в фокусе, т.е. в текущем View).
 */
#define WORLD_SIZE_ON_SCREEN_PART 0.25f /* 25% */


#include "World.hpp"
#include "../Game.hpp"
#include "WorldLoader.hpp"

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

    void World::focusCamera(float worldX, float worldY) {
        float worldWidthPixels  = worldData->width  * worldData->tileSize; // NOLINT(cppcoreguidelines-narrowing-conversions)
        float worldHeightPixels = worldData->height * worldData->tileSize; // NOLINT(cppcoreguidelines-narrowing-conversions)

        window->setView(sf::View(
                sf::Vector2f(
                        worldX * worldData->tileSize,  // NOLINT(cppcoreguidelines-narrowing-conversions)
                        worldY * worldData->tileSize   // NOLINT(cppcoreguidelines-narrowing-conversions)
                ),
                sf::Vector2f(
                        WORLD_SIZE_ON_SCREEN_PART * worldWidthPixels,
                        WORLD_SIZE_ON_SCREEN_PART * worldHeightPixels
                )
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
                std::wcerr << L"Failed to update dimension (world load error)" << std::endl;
                Game::instance().shutdown();

                return;
            }

            std::wcout << L"Updated dimension successfully" << std::endl;

            // Изначально фокусируем камеру по центру мира.
            centerCamera();
        }
    }

}
