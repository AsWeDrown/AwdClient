/**
 * Размер текстур всех тайлов (ширина и длина, в пикселях).
 */
#define WORLD_TILEMAP_TILE_SIZE 16

/**
 * Ширина и высота мира (в тайлах). Наиболее оптимальное отношение ширины к длине - 16:9.
 */
#define WORLD_SIZE_WIDTH 128
#define WORLD_SIZE_HEIGHT 72

/**
 * Часть мира (%/100), отображаемая на экрана (находящаяся в фокусе, т.е. в текущем View).
 */
#define WORLD_SIZE_ON_SCREEN_PART 0.225f /* 22.5% */


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
        // Изначально фокусируем камеру по центру мира.
        centerCamera();

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
        if (worldVertices != nullptr) {
            sf::RenderStates states(Game::instance().getTextures()->worldTileMap.get());
            window->draw(*worldVertices, states);
        }

        // ... а затем уже всё остальное.
        Drawable::draw();
    }

    void World::focusCamera(float worldX, float worldY) {
        window->setView(sf::View(
                sf::Vector2f(worldX, worldY),
                sf::Vector2f(
                        WORLD_SIZE_ON_SCREEN_PART * WORLD_SIZE_WIDTH  * WORLD_TILEMAP_TILE_SIZE,
                        WORLD_SIZE_ON_SCREEN_PART * WORLD_SIZE_HEIGHT * WORLD_TILEMAP_TILE_SIZE
                )
        ));
    }

    void World::centerCamera() {
        focusCamera(
                window->getSize().x / 2.0f, // NOLINT(cppcoreguidelines-narrowing-conversions
                window->getSize().y / 2.0f  // NOLINT(cppcoreguidelines-narrowing-conversions
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
                std::wcerr << L"Failed to update dimension (world load error)" << std::endl;
                Game::instance().shutdown();

                return;
            }

            std::wcout << L"Updated dimension successfully" << std::endl;
        }
    }

}
