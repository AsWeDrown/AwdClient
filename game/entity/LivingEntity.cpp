#include "LivingEntity.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    LivingEntity::LivingEntity(uint32_t entityType, id_type entityId)
                               : Entity(entityType, entityId) {}

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   События Drawable
    ///////////////////////////////////////////////////////////////////////////////////////////

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
