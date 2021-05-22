#pragma once


#include <deque>
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <atomic>
#include "../graphics/common/Drawable.hpp"
#include "Entities.hpp"
#include "EntityStateSnapshot.hpp"

namespace awd::game {

    class Entity : public Drawable {
    private:
        std::mutex interpBufferMutex;

    protected:
        std::atomic<bool> registered = false; // пока false, никаких события Drawable не должны обрабатываться

        uint32_t entityType; // тип сущности

        id_type entityId; // ID этой сущности в её текущем мире (ID графики на экране - this->id (от Drawable))

        float posX         = 0.0f, // координата X в мире (В ТАЙЛАХ) (координата на экране - this->x (от Drawable))
              posY         = 0.0f, // координата Y в мире (В ТАЙЛАХ) (координата на экране - this->y (от Drawable))
              faceAngle    = 0.0f, // угол, указывающий, в какую сторону сейчас смотрит эта сущность
              spriteWidth  = 0.0f, // ширина спрайта (модельки) (В ТАЙЛАХ)
              spriteHeight = 0.0f; // высота спрайта (модельки) (В ТАЙЛАХ)

        float lastTickDeltaX         = 0.0f,
              lastTickDeltaY         = 0.0f,
              lastTickDeltaFaceAngle = 0.0f;

        bool isControlled = false; // true только для игрока, которым мы управляем; false - для всех остальных

        std::shared_ptr<sf::Sprite> entitySprite = nullptr;

        std::deque<EntityStateSnapshot> interpolationBuffer;

        void internalSetPosition(float newX, float newY, float newFaceAngle);

    public:
        Entity(uint32_t entityType, id_type entityId);

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   События Drawable
        ///////////////////////////////////////////////////////////////////////////////////////////

        void onRegister() override;

        void keyPressed(const sf::Event::KeyEvent& event) override;
        void mousePressed(const sf::Event::MouseButtonEvent& event) override;
        void update() override;
        void draw() override;

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Геттеры
        ///////////////////////////////////////////////////////////////////////////////////////////

        uint32_t getEntityType     () const;
        id_type  getEntityId       () const;
        float    getPosX           () const;
        float    getPosY           () const;
        float    getFaceAngle      () const;
        bool     isControlledPlayer() const;
        bool     movedLastTick     () const;

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Сеттеры (скорее даже "апдейтеры")
        ///////////////////////////////////////////////////////////////////////////////////////////

        virtual void setPosition(float newX,   float newY,   float newFaceAngle);
                void move       (float deltaX, float deltaY, float deltaFaceAngle);

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Игровые события
        ///////////////////////////////////////////////////////////////////////////////////////////

        virtual void positionChanged(float oldX, float oldY, float newX, float newY);
        virtual void rotationChanged(float oldFaceAngle,     float newFaceAngle    );

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Утилити-методы
        ///////////////////////////////////////////////////////////////////////////////////////////

        sf::Vector2f calcPosOnScreen() const;

        std::shared_ptr<sf::Sprite> createScaledSprite(
                const std::shared_ptr<sf::Texture>& texture) const;

        static id_type entityIdToDrawableId(id_type entityId);
        static id_type drawableIdToEntityId(id_type drawableId);
    };

}

