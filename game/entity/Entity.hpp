#pragma once


#include <cstdint>
#include <SFML/Graphics.hpp>
#include "../graphics/common/Drawable.hpp"

namespace awd::game {

    class Entity : public Drawable {
    protected:
        uint32_t entityType; // тип сущности

        id_type entityId; // ID этой сущности в её текущем мире (ID графики на экране - this->id (от Drawable))

        float posX         = 0.0f, // координата X в мире (В ТАЙЛАХ) (координата на экране - this->x (от Drawable))
              posY         = 0.0f, // координата Y в мире (В ТАЙЛАХ) (координата на экране - this->y (от Drawable))
              faceAngle    = 0.0f, // угол, указывающий, в какую сторону сейчас смотрит эта сущность
              spriteWidth  = 0.0f, // ширина спрайта (модельки) (В ТАЙЛАХ)
              spriteHeight = 0.0f; // высота спрайта (модельки) (В ТАЙЛАХ)

        std::shared_ptr<sf::Sprite> entitySprite = nullptr;

    public:
        Entity(uint32_t entityType, id_type entityId);

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   События Drawable
        ///////////////////////////////////////////////////////////////////////////////////////////

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Геттеры
        ///////////////////////////////////////////////////////////////////////////////////////////

        uint32_t               getEntityType() const;
        id_type                getEntityId  () const;
        float                  getPosX      () const;
        float                  getPosY      () const;
        float                  getFaceAngle () const;

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Сеттеры (скорее даже "апдейтеры")
        ///////////////////////////////////////////////////////////////////////////////////////////

        void setPosition(float newX,   float newY  );
        void move       (float deltaX, float deltaY);
        void setRotation(float newFaceAngle        );
        void rotate     (float deltaFaceAngle      );

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Игровые события
        ///////////////////////////////////////////////////////////////////////////////////////////

        virtual void positionUpdated(float oldX, float oldY, float newX, float newY);

        virtual void rotationUpdated(float oldFaceAngle, float newFaceAngle);

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Утилити-методы
        ///////////////////////////////////////////////////////////////////////////////////////////

        sf::Vector2f calcPosOnScreen() const;

        void scaleSprite(const std::shared_ptr<sf::Sprite>& sprite);

        static id_type entityIdToDrawableId(id_type entityId);
        static id_type drawableIdToEntityId(id_type drawableId);
    };

}

