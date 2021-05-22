#include "Entity.hpp"
#include "../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PROTECTED
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void Entity::internalSetPosition(float newX, float newY, float newFaceAngle) {
        float oldX         = this->posX;
        float oldY         = this->posY;
        float oldFaceAngle = this->faceAngle;

        lastTickDeltaX         = newX         - oldX;
        lastTickDeltaY         = newY         - oldY;
        lastTickDeltaFaceAngle = newFaceAngle - oldFaceAngle;

        bool posChanged = lastTickDeltaX         != 0.0f || lastTickDeltaY != 0.0f;
        bool rotChanged = lastTickDeltaFaceAngle != 0.0f;

        if (posChanged) {
            this->posX = newX;
            this->posY = newY;

            float tileSize = Game::instance().currentWorld()->getWorldData()->displayTileSize; // NOLINT(cppcoreguidelines-narrowing-conversions)

            this->x = posX * tileSize;
            this->y = posY * tileSize;

            if (entitySprite != nullptr)
                entitySprite->setPosition(x, y);

            positionChanged(oldX, oldY, posX, posY);
        }

        if (rotChanged) {
            this->faceAngle = newFaceAngle;
            rotationChanged(oldFaceAngle, newFaceAngle);
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

    void Entity::onRegister() {
        registered = true;
    }

    void Entity::keyPressed(const sf::Event::KeyEvent& event) {
        if (!registered)
            return;

        Drawable::keyPressed(event);
    }

    void Entity::mousePressed(const sf::Event::MouseButtonEvent& event) {
        if (!registered)
            return;

        Drawable::mousePressed(event);
    }

    void Entity::update() {
        if (!registered)
            return;

        Drawable::update();

        if (!isControlled) {
            std::unique_lock<std::mutex> lock(interpBufferMutex);

            if (interpolationBuffer.size() > Game::instance().getConfigs()->physics.interpBufSizeThreshold)
                // Кажется, мы получили кучу пакетов от сервера, не успев их толком обработать.
                // Скорее всего, клиент ненадолго завис (очень сильно залагал). Стираем наиболее
                // старые данные, чтобы не отставать от сервера сильнее положенного.
                while (interpolationBuffer.size() > Game::instance().getConfigs()->physics.interpDelay)
                    interpolationBuffer.pop_front();

            // Интерполируем позиции сущности из прошлого. Используем ">=", а не "==", т.к. размер
            // буфера интерполяции может быть и больше interpDelay (но не больше interpBufSizeThreshold).
            // Если тупо удалять все снимки состояний старее interpDelay, то при движении все существа
            // будут периодически дёргаться и/или подвисать (выглядит это совершенно неиграбельно).
            if (interpolationBuffer.size() >= Game::instance().getConfigs()->physics.interpDelay) {
                EntityStateSnapshot oldestSnapshot = interpolationBuffer.front();
                interpolationBuffer.pop_front();
                internalSetPosition(oldestSnapshot.posX, oldestSnapshot.posY, oldestSnapshot.faceAngle);
            }
        }
    }

    void Entity::draw() {
        if (!registered)
            return;

        Drawable::draw();

        if (entitySprite != nullptr)
            window->draw(*entitySprite);
        else
            std::wcerr << L"Cannot draw entity " << entityId << std::endl;
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

    bool Entity::isControlledPlayer() const {
        return isControlled;
    }

    bool Entity::movedLastTick() const {
        return lastTickDeltaX         != 0.0f
            || lastTickDeltaY         != 0.0f
            || lastTickDeltaFaceAngle != 0.0f;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   Сеттеры (скорее даже "апдейтеры")
    ///////////////////////////////////////////////////////////////////////////////////////////

    void Entity::setPosition(float newX, float newY, float newFaceAngle) {
        if (isControlled)
            // Обновляем позицию сразу.
            internalSetPosition(newX, newY, newFaceAngle);
        else {
            // Добавляем позицию в буфер интерполяции.
            // TODO: линейная интерполяция позиций при слишком большом расстоянии (быстрая прокрутка событий прошлого).
            EntityStateSnapshot newSnapshot;

            newSnapshot.posX      = newX;
            newSnapshot.posY      = newY;
            newSnapshot.faceAngle = newFaceAngle;

            std::unique_lock<std::mutex> lock(interpBufferMutex);
            interpolationBuffer.push_back(newSnapshot);
        }
    }

    void Entity::move(float deltaX, float deltaY, float deltaFaceAngle) {
        setPosition(posX + deltaX, posY + deltaY, faceAngle + deltaFaceAngle);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   Игровые события
    ///////////////////////////////////////////////////////////////////////////////////////////

    void Entity::positionChanged(float oldX, float oldY, float newX, float newY) {}

    void Entity::rotationChanged(float oldFaceAngle, float newFaceAngle) {}

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

    std::shared_ptr<sf::Sprite> Entity::createScaledSprite(
            const std::shared_ptr<sf::Texture>& texture) const {
        auto sprite = std::make_shared<sf::Sprite>(*texture);

        float tileSize           = Game::instance().currentWorld()->getWorldData()->displayTileSize; // NOLINT(cppcoreguidelines-narrowing-conversions)
        float spriteWidthPixels  = spriteWidth  * tileSize;
        float spriteHeightPixels = spriteHeight * tileSize;

        sf::FloatRect baseBounds = sprite->getGlobalBounds();

        sprite->setScale( // устанавливаем нужный нам размер спрайта (в соответствии с размером Entity в тайлах)
                spriteWidthPixels  / baseBounds.width,
                spriteHeightPixels / baseBounds.height
        );

        return sprite;
    }

    id_type Entity::entityIdToDrawableId(id_type entityId) {
        return entityId + ID_SCREEN_PLAY_WORLD_ENTITY;
    }

    id_type Entity::drawableIdToEntityId(id_type drawableId) {
        return drawableId - ID_SCREEN_PLAY_WORLD_ENTITY;
    }

}
