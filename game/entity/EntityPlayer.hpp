#pragma once


#include <atomic>
#include <map>
#include "LivingEntity.hpp"

namespace awd::game {

    class PlayerStateSnapshot {
    public:
        float posX      = 0.0f,
              posY      = 0.0f,
              faceAngle = 0.0f;
    };

    bool operator ==(const PlayerStateSnapshot& a, const PlayerStateSnapshot& b);
    bool operator !=(const PlayerStateSnapshot& a, const PlayerStateSnapshot& b);

    class PlayerInputs {
    public:
        static constexpr uint32_t INPUT_MOVING_LEFT  = 0b1;
        static constexpr uint32_t INPUT_MOVING_RIGHT = 0b10;
        static constexpr uint32_t INPUT_MOVING_UP    = 0b100;
        static constexpr uint32_t INPUT_MOVING_DOWN  = 0b1000;

        uint32_t inputsBitfield = 0;

        void reset();

        void setMovingLeft (bool enable);
        void setMovingRight(bool enable);
        void setMovingUp   (bool enable);
        void setMovingDown (bool enable);

        /**
         * "Оптимизирует" пользовательский ввод. Например, если были одновременно нажаты
         * клавиши "идти влево" и "идти вправо", то оба флага будут сняты, и игрок не будет
         * никуда двигаться. Таким образом, результат применения этого ввода остаётся неизменным,
         * однако количество установленных битов в inputsBitfield сокращается. Это полезно для
         * снижения количества передаваемых данных в долгосрочной перспективе, а также при обработке
         * ввода (иногда при обработке нужно обращаться к данным мира, что довольно дорого: если игрок
         * одновременно жмёт, скажем, кнопки "ползти по лестнице вверх" и "вниз", то нет смысла тратить
         * ресурсы на проверку того, находится ли он сейчас на лестнице - ведь по сути он не двигается
         * по ней вообще).
         */
        void canonicalize();
        void onlyOneOf(uint32_t inputsCombination);

        bool empty() const;

        bool movingLeft () const;
        bool movingRight() const;
        bool movingUp   () const;
        bool movingDown () const;
    };

    class MoveMechanics {
    public:
        uint32_t localSequence = 0;

        PlayerInputs playerInputs;

        bool     climbing             = false;
        uint32_t midairTicks          =     0;
        float    lastTickFallDistance =  0.0f,
                 fallDistance         =  0.0f;

        MoveMechanics();

        MoveMechanics(uint32_t localSequence, PlayerInputs playerInputs,
                      bool climbing, uint32_t midairTicks, float lastTickFallDistance, float fallDistance);

        bool empty() const;

        void reset();
    };

    class EntityPlayer : public LivingEntity {
    private:
        std::mutex mutex;

        sf::Clock stepSoundClock;

        std::map<uint32_t, std::shared_ptr<sf::Sprite>> playerSprites;

        uint32_t remainingAnimTicks = 0;
        uint32_t prevAnim           = Entities::EntityPlayer::ANIM_BASE_STILL_FRONT;
        uint32_t prevPrevAnim       = Entities::EntityPlayer::ANIM_BASE_STILL_FRONT;
        uint32_t currentAnim        = Entities::EntityPlayer::ANIM_BASE_STILL_FRONT;

        static const uint32_t TILE_INTERACT_LEFT_CLICK  = 1;
        static const uint32_t TILE_INTERACT_RIGHT_CLICK = 2;

        MoveMechanics currentMoveMechanics;

        // Номер (sequence number) последнего пакета UpdatePlayerInputs,
        // который УЖЕ УЧЁЛ И ОБРАБОТАЛ (а не просто получил!) сервер.
        // Используется для прогнозирования передвижений (для controlled).
        std::atomic<uint32_t> lastSrvProcInputs = 0;

        std::deque<MoveMechanics> recentMoveMechanicsSnapshots;

        void prepareSprites();

        void updateMoveMechanics();

        void updateGravity(MoveMechanics& moveMechanics) const;

        void updatePlayerInputs(MoveMechanics& moveMechanics) const;

        void updateAnimation();

        void applyMoveMechanicsSnapshot(const MoveMechanics& snapshot,
                                        PosUpdateStrategy strategy = PosUpdateStrategy::NORMAL);

        void saveMoveMechanicsSnapshot(const MoveMechanics& snapshot);

        PlayerStateSnapshot takeStateSnapshot() const;

    public:
        uint32_t     playerId;
        std::wstring name;
        uint32_t     character;

        EntityPlayer(id_type entityId, uint32_t playerId,
                     const std::wstring& name, uint32_t character);

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   События Drawable
        ///////////////////////////////////////////////////////////////////////////////////////////

        void keyPressed(const sf::Event::KeyEvent& event) override;

        void mousePressed(const sf::Event::MouseButtonEvent &event) override;

        void update() override;

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Сеттеры (скорее даже "апдейтеры")
        ///////////////////////////////////////////////////////////////////////////////////////////

        void setLastSrvProcInputs(uint32_t ack);

        void setGravityData(uint32_t midairTicks,
                            float lastTickFallDistance, float fallDistance);

        void setPosition(float newX, float newY, float newFaceAngle) override; // прогноз передвижения

        void turnSprite(const std::shared_ptr<sf::Sprite>& sprite, float newFaceAngle);

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Игровые события
        ///////////////////////////////////////////////////////////////////////////////////////////

        void positionChanged(float oldX, float oldY, float newX, float newY) override;

        void rotationChanged(float oldFaceAngle, float newFaceAngle) override;

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Утилити-методы
        ///////////////////////////////////////////////////////////////////////////////////////////

        void playMoveSound() override;
    };

}

