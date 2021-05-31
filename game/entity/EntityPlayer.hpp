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

        uint32_t inputsBitfield = 0;

        void reset();

        void setMovingLeft (bool enable);
        void setMovingRight(bool enable);

        void canonicalize();

        bool empty() const;

        bool movingLeft () const;
        bool movingRight() const;
    };

    class MoveMechanics {
    public:
        uint32_t localSequence = 0;

        PlayerInputs playerInputs;

        uint32_t midairTicks          =    0;
        float    lastTickFallDistance = 0.0f,
                 fallDistance         = 0.0f;

        MoveMechanics();

        MoveMechanics(uint32_t localSequence, PlayerInputs playerInputs,
                      uint32_t midairTicks, float lastTickFallDistance, float fallDistance);

        bool empty() const;
    };

    class EntityPlayer : public LivingEntity {
    private:
        std::mutex mutex;

        std::map<uint32_t, std::shared_ptr<sf::Sprite>> playerSprites;

        uint32_t remainingAnimTicks = 0;
        uint32_t prevAnim           = Entities::EntityPlayer::ANIM_BASE_STILL_FRONT;
        uint32_t prevPrevAnim       = Entities::EntityPlayer::ANIM_BASE_STILL_FRONT;
        uint32_t currentAnim        = Entities::EntityPlayer::ANIM_BASE_STILL_FRONT;

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
    };

}

