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

    id_type Entity::entityIdToDrawableId(id_type entityId) {
        return ID_SCREEN_PLAY_WORLD + entityId * ID_OTHER;
    }

    id_type Entity::drawableIdToEntityId(id_type drawableId) {
        return (drawableId - ID_SCREEN_PLAY_WORLD) / ID_OTHER;
    }

}
