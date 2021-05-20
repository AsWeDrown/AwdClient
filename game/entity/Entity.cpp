#define INTERPOLATION_BUFFER_SIZE 5 /* 0.2 секунды (при 25 TPS) */


#include "Entity.hpp"
#include "../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void Entity::internalSetPosition(float newX, float newY) {
        if (this->posX != newX || this->posY != newY) {
            // Понадобится для реакции на обновление позиции.
            float oldX = this->posX;
            float oldY = this->posY;

            // Новые координаты в мире.
            this->posX = newX;
            this->posY = newY;

            // Новые координаты на экране (в фокусе (View)).
            float tileSize = Game::instance().currentWorld()->getWorldData()->displayTileSize; // NOLINT(cppcoreguidelines-narrowing-conversions)

            this->x = posX * tileSize;
            this->y = posY * tileSize;

            if (entitySprite != nullptr)
                // Обновляем позицию модельки.
                entitySprite->setPosition(x, y);

            // Реакция на обновления позиции
            positionUpdated(oldX, oldY, posX, posY);
        }
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    Entity::Entity(uint32_t entityType, id_type entityId)
                   : Drawable(entityIdToDrawableId(entityId)) {
        this->entityType = entityType;
        this->entityId   = entityId;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   События Drawable
    ///////////////////////////////////////////////////////////////////////////////////////////

    void Entity::keyPressed(const sf::Event::KeyEvent& event) {
        Drawable::keyPressed(event);
    }

    void Entity::mousePressed(const sf::Event::MouseButtonEvent& event) {
        Drawable::mousePressed(event);
    }

    void Entity::update() {
        Drawable::update();

        if (!isControlled) {
            while (interpolationBuffer.size() > INTERPOLATION_BUFFER_SIZE)
                // Кажется, мы получили кучу пакетов от сервера, не успев их толком обработать.
                // Скорее всего, клиент ненадолго завис / очень сильно залагал. Стираем наиболее
                // старые данные, чтобы не отставать от сервера сильнее положенного.
                interpolationBuffer.pop_front();

            if (interpolationBuffer.size() == INTERPOLATION_BUFFER_SIZE) {
                EntityStateSnapshot oldestSnapshot = interpolationBuffer.front();
                interpolationBuffer.pop_front();
                std::wcerr << L"interpolate " << oldestSnapshot.posX << L"," << oldestSnapshot.posY << L" | "
                           << interpolationBuffer.size() << std::endl;
                internalSetPosition(oldestSnapshot.posX, oldestSnapshot.posY);
            }
        }
    }

    void Entity::draw() {
        Drawable::draw();

        if (entitySprite != nullptr)
            window->draw(*entitySprite);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   Геттеры
    ///////////////////////////////////////////////////////////////////////////////////////////

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

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   Сеттеры (скорее даже "апдейтеры")
    ///////////////////////////////////////////////////////////////////////////////////////////

    void Entity::setPosition(float newX, float newY) {
        if (isControlled)
            internalSetPosition(newX, newY);
        else {
            EntityStateSnapshot newSnapshot;

            newSnapshot.posX      = newX;
            newSnapshot.posY      = newY;
            newSnapshot.faceAngle = faceAngle;

            interpolationBuffer.push_back(newSnapshot);
        }
    }

    void Entity::move(float deltaX, float deltaY) {
        setPosition(posX + deltaX, posY + deltaY);
    }

    void Entity::setRotation(float newFaceAngle) {
        if (this->faceAngle != newFaceAngle) {
            // Понадобится для реакции на обновление угла поворота.
            float oldFaceAngle = this->faceAngle;

            // Новая ориентация в мире.
            this->faceAngle = newFaceAngle;

            if (entitySprite != nullptr)
                // Обновляем ориентацию модельки.
                entitySprite->setRotation(faceAngle);

            // Реакция на обновление угла поворота.
            rotationUpdated(oldFaceAngle, faceAngle);
        }
    }

    void Entity::rotate(float deltaFaceAngle) {
        setRotation(faceAngle + deltaFaceAngle);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   Игровые события
    ///////////////////////////////////////////////////////////////////////////////////////////

    void Entity::positionUpdated(float oldX, float oldY, float newX, float newY) {}

    void Entity::rotationUpdated(float oldFaceAngle, float newFaceAngle) {}

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   Утилити-методы
    ///////////////////////////////////////////////////////////////////////////////////////////

    sf::Vector2f Entity::calcPosOnScreen() const {
        // posX и posY - это координаты в тайлах - в системе координат, описанной здесь:
        // https://github.com/AsWeDrown/awd-protocol/wiki/%D0%A1%D0%B8%D1%81%D1%82%D0%B5%D0%BC%D0%B0-%D0%BA%D0%BE%D0%BE%D1%80%D0%B4%D0%B8%D0%BD%D0%B0%D1%82
        // Умножая эти значения на размер тайла в пикселях (tileSize), получаем глобальные
        // координаты сущности в мире, но уже в пикселях. Затем используем метод SFML
        // mapCoordsToPixel для преобразования этих глобальных координат в локальыне,
        // т.е. в координаты конкретно внутри пользовательского окна (фокуса (View)).
        float tileSize = Game::instance().currentWorld()->getWorldData()->displayTileSize; // NOLINT(cppcoreguidelines-narrowing-conversions)

        sf::Vector2i pixelPos = window->mapCoordsToPixel(sf::Vector2f(
                posX * tileSize,
                posY * tileSize
        ));

        return sf::Vector2f(
                pixelPos.x, // NOLINT(cppcoreguidelines-narrowing-conversions)
                pixelPos.y  // NOLINT(cppcoreguidelines-narrowing-conversions)
        );
    }

    void Entity::scaleSprite(const std::shared_ptr<sf::Sprite>& sprite) const {
        float tileSize           = Game::instance().currentWorld()->getWorldData()->displayTileSize; // NOLINT(cppcoreguidelines-narrowing-conversions)
        float spriteWidthPixels  = spriteWidth  * tileSize;
        float spriteHeightPixels = spriteHeight * tileSize;

        sf::FloatRect baseBounds = sprite->getGlobalBounds();

        sprite->setScale( // устанавливаем нужный нам размер спрайта
                spriteWidthPixels  / baseBounds.width,
                spriteHeightPixels / baseBounds.height
        );
    }

    id_type Entity::entityIdToDrawableId(id_type entityId) {
        return ID_SCREEN_PLAY_WORLD + entityId * ID_OTHER;
    }

    id_type Entity::drawableIdToEntityId(id_type drawableId) {
        return (drawableId - ID_SCREEN_PLAY_WORLD) / ID_OTHER;
    }

}
