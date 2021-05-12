#pragma once


#include <cstdint>
#include <SFML/Graphics.hpp>
#include "../graphics/common/Drawable.hpp"

namespace awd::game {

    class Entity : public Drawable {
    protected:
        id_type entityId; // Entity ID в мире

        std::shared_ptr<sf::Sprite> entitySprite = nullptr;

    public:
        float posX         = 0.0f, // координата X в мире
              posY         = 0.0f, // координата Y в мире
              spriteWidth  = 0.0f, // ширина спрайта (модельки)
              spriteHeight = 0.0f; // высота спрайта (модельки)

        explicit Entity(id_type entityId);

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        static id_type entityIdToDrawableId(id_type entityId);
        static id_type drawableIdToEntityId(id_type drawableId);
    };

}

