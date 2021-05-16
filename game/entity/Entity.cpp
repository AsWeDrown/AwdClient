#include "Entity.hpp"
#include "../Game.hpp"
#include "../graphics/play/PlayScreen.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Entity::Entity(uint32_t entityType, id_type entityId)
                   : Drawable(entityIdToDrawableId(entityId)) {
        this->entityType = entityType;
        this->entityId = entityId;
    }

    void Entity::keyPressed(const sf::Event::KeyEvent& event) {
        Drawable::keyPressed(event);
    }

    void Entity::mousePressed(const sf::Event::MouseButtonEvent& event) {
        Drawable::mousePressed(event);
    }

    void Entity::update() {
        Drawable::update();
    }

    void Entity::draw() {
        Drawable::draw();

        if (entitySprite != nullptr)
            window->draw(*entitySprite);
    }

    uint32_t Entity::getEntityType() const {
        return entityType;
    }

    id_type Entity::getEntityId() const {
        return entityId;
    }

    float Entity::getPosX() const {
        return posX;
    }

    float Entity::getPosY() const {
        return posY;
    }

    float Entity::getFaceAngle() const {
        return faceAngle;
    }

    void Entity::setPosition(float newX, float newY) {
        if (this->posX != newX || this->posY != newY) {
            // Новые координаты в мире.
            this->posX = newX;
            this->posY = newY;

            // Новые координаты на экране (в фокусе (View)).
            sf::Vector2f screenPos = calcPosOnScreen();

            this->x = screenPos.x;
            this->y = screenPos.y;

            if (entitySprite != nullptr)
                // Обновляем позицию модельки.
                entitySprite->setPosition(x, y);
        }
    }

    void Entity::move(float deltaX, float deltaY) {
        setPosition(posX + deltaX, posY + deltaY);
    }

    void Entity::setRotation(float newFaceAngle) {
        if (this->faceAngle != newFaceAngle) {
            // Новая ориентация в мире.
            this->faceAngle = newFaceAngle;

            if (entitySprite != nullptr)
                // Обновляем ориентацию модельки.
                entitySprite->setRotation(faceAngle);
        }
    }

    void Entity::rotate(float deltaFaceAngle) {
        setRotation(faceAngle + deltaFaceAngle);
    }

    sf::Vector2f Entity::calcPosOnScreen() const {
        if (auto playScreen = std::dynamic_pointer_cast
                <PlayScreen>(Game::instance().getCurrentScreen())) {
            if (auto worldData = playScreen->getWorld()->getWorldData()) {
                // posX и posY - это координаты в тайлах - в системе координат, описанной здесь:
                // https://github.com/AsWeDrown/awd-protocol/wiki/%D0%A1%D0%B8%D1%81%D1%82%D0%B5%D0%BC%D0%B0-%D0%BA%D0%BE%D0%BE%D1%80%D0%B4%D0%B8%D0%BD%D0%B0%D1%82
                // Умножая эти значения на размер тайла в пикселях (tileSize), получаем глобальные
                // координаты сущности в мире, но уже в пикселях. Затем используем метод SFML
                // mapCoordsToPixel для преобразования этих глобальных координат в локальыне,
                // т.е. в координаты конкретно внутри пользовательского окна (фокуса (View)).
                uint32_t     tileSize = worldData->tileSize;
                sf::Vector2i pixelPos = window->mapCoordsToPixel(sf::Vector2f(
                        tileSize * posX, // NOLINT(cppcoreguidelines-narrowing-conversions)
                        tileSize * posY  // NOLINT(cppcoreguidelines-narrowing-conversions)
                ));

                return sf::Vector2f(
                        pixelPos.x, // NOLINT(cppcoreguidelines-narrowing-conversions)
                        pixelPos.y  // NOLINT(cppcoreguidelines-narrowing-conversions)
                );
            }
        }

        return sf::Vector2f(posX, posY); // подстраховка
    }

    id_type Entity::entityIdToDrawableId(id_type entityId) {
        return ID_SCREEN_PLAY_WORLD + entityId * ID_OTHER;
    }

    id_type Entity::drawableIdToEntityId(id_type drawableId) {
        return (drawableId - ID_SCREEN_PLAY_WORLD) / ID_OTHER;
    }

}
