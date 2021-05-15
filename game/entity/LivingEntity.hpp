#pragma once


#include "Entity.hpp"

namespace awd::game {

    class LivingEntity : public Entity {
    public:
        float maxHealth     = 0.0f; // максимальный запас здоровья этой сущности (в хитпоинтах) (не должен меняться)
        float currentHealth = 0.0f; // текущий запас здоровья этой сущности (в хитпоинтах)

        LivingEntity(uint32_t entityType, id_type entityId);

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
    };

}

