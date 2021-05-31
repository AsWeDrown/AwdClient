#pragma once


#include <deque>
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <atomic>
#include "../graphics/common/Drawable.hpp"
#include "Entities.hpp"
#include "EntityStateSnapshot.hpp"
#include "../world/Collidable.hpp"

namespace awd::game {

    enum class PosUpdateStrategy {
        /**
         * Устанавливает новое местоположение в мире, обновляет позицию спрайта
         * (модельки) и вызывает события обновления (positionChanged/rotationChanged).
         *
         * Релокация спрайта и вызов нужных событий обновления произойдёт только в случае,
         * если новая позиция отличается от текущей (хотя бы одна координата/угол).
         *
         * Используется почти во всех случаях.
         */
        NORMAL,

        /**
         * Просто устанавливает новое местоположение в мире, не обновляя позицию спрайта
         * (модельки) и не вызывая события обновления (positionChanged/rotationChanged).
         *
         * Используется для применения "временных" ("промежуточных") позиций при локальном обновлении
         * позиции управляемого (isControlled) игрока (EntityPlayer). Это нужно, чтобы моделька игрока
         * и/или камера визуально не дёргалась при многократном применении нескольких снимков сохранённых
         * MoveMechanics подряд (например, когда сервер "отстаёт" от прогнозов клиента на несколько тиков).
         */
        SILENT,

        /**
         * Устанавливает новое местоположение в мире, обновляет позицию спрайта
         * (модельки) и вызывает события обновления (positionChanged/rotationChanged).
         *
         * Релокация спрайта и вызов нужных событий обновления произойдёт в любом случае,
         * т.е. даже если новая позиция полностью совпадает с текущей.
         *
         * Используется для корректного отображения "итоговой" ("конечной") позиции при локальном
         * обновлении управляемого (isControlled) игрока (EntityPlayer). Это нужно, т.к. не всегда
         * последний применённый снимок MoveMechanics отражает самые последние изменения - их может
         * отражать и предыдущий снимок - однако он не отображался (SILENT). В итоге игрок может
         * иногда, например, зависать в воздухе до тех пор, пока не сдвинется с места. Чтобы этого
         * избежать, нужно в конце использовать FORCED для "актуализации" позиции на экране пользователя.
         */
        FORCED
    };

    class Entity :
            public Drawable,
            public Collidable {
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

        /**
         * @param silent false (default) -
         *                                 Используется почти во всех случаях, кроме [см. ниже].
         *
         *               true            - Просто установить новое местоположение в мире - без
         *                                 обновления позиции спрайта (модельки) и вызова событий
         *                                 обновления. Используется для применения "временных"
         *                                 ("промежуточных") позиций при локальном обновлении
         *                                 позиции управляемого игрока (EntityPlayer; isControlled).
         *                                 Это нужно, чтобы моделька игрока/камера визуально не
         *                                 дёргалась при многократном применении нескольких снимков
         *                                 сохранённых DeltaPosition.
         */
        void internalSetPosition(float newX, float newY, float newFaceAngle,
                                 PosUpdateStrategy strategy = PosUpdateStrategy::NORMAL);

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

        uint32_t     getEntityType     () const;
        id_type      getEntityId       () const;
        float        getPosX           () const;
        float        getPosY           () const;
        float        getFaceAngle      () const;
        bool         isControlledPlayer() const;
        bool         movedLastTick     () const;
        BoundingBox  getBoundingBox    () const override;

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

