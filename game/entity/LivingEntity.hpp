#pragma once


#include "Entity.hpp"

namespace awd::game {

    class LivingEntity : public Entity {
    public:
        float maxHealth     = 0.0f;
        float currentHealth = 0.0f;

        explicit LivingEntity(id_type entityId);

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
    };

}

