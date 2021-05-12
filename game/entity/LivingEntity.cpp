#include "LivingEntity.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    LivingEntity::LivingEntity(id_type entityId) : Entity(entityId) {}

    void LivingEntity::keyPressed(const sf::Event::KeyEvent& event) {
        Entity::keyPressed(event);
    }

    void LivingEntity::mousePressed(const sf::Event::MouseButtonEvent& event) {
        Entity::mousePressed(event);
    }

    void LivingEntity::update() {
        Entity::update();
    }

}
