#include "Entity.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Entity::Entity(id_type entityId)
                   : Drawable(entityIdToDrawableId(entityId)) {
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

    uint32_t Entity::getEntityId() const {
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

            // Новые координаты на экране.
            sf::Vector2i pixelPos = window->
                    mapCoordsToPixel(sf::Vector2f(posX, posY));

            this->x = pixelPos.x; // NOLINT(cppcoreguidelines-narrowing-conversions)
            this->y = pixelPos.y; // NOLINT(cppcoreguidelines-narrowing-conversions)

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

    id_type Entity::entityIdToDrawableId(id_type entityId) {
        return ID_SCREEN_PLAY_WORLD + entityId * ID_OTHER;
    }

    id_type Entity::drawableIdToEntityId(id_type drawableId) {
        return (drawableId - ID_SCREEN_PLAY_WORLD) / ID_OTHER;
    }

}
