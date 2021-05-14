#pragma once


#include <cstdint>
#include <SFML/Graphics.hpp>
#include "../graphics/common/Drawable.hpp"

namespace awd::game {

    class Entity : public Drawable {
    protected:
        id_type entityId; // Entity ID в мире

        float posX         = 0.0f, // координата X в мире (координата на экране - this->x (от Drawable))
              posY         = 0.0f, // координата Y в мире (координата на экране - this->y (от Drawable))
              faceAngle    = 0.0f, // угол, указывающий, в какую сторону сейчас смотрит эта сущность
              spriteWidth  = 0.0f, // ширина спрайта (модельки)
              spriteHeight = 0.0f; // высота спрайта (модельки)

        std::shared_ptr<sf::Sprite> entitySprite = nullptr;

    public:
        explicit Entity(id_type entityId);

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        uint32_t getEntityId () const;
        float    getPosX     () const;
        float    getPosY     () const;
        float    getFaceAngle() const;

        void setPosition(float newX,   float newY  );
        void move       (float deltaX, float deltaY);

        void setRotation(float newFaceAngle  );
        void rotate     (float deltaFaceAngle);

        static id_type entityIdToDrawableId(id_type entityId);
        static id_type drawableIdToEntityId(id_type drawableId);
    };

}

