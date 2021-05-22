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

        uint32_t localSequence  = 0,
                 inputsBitfield = 0;

        PlayerInputs();
        PlayerInputs(uint32_t localSequence, uint32_t inputsBitfield);

        void reset();

        void setMovingLeft (bool enable);
        void setMovingRight(bool enable);

        bool empty() const;

        bool movingLeft () const;
        bool movingRight() const;

        void apply(PlayerStateSnapshot& playerState) const;
    };

    class EntityPlayer : public LivingEntity {
    private:
        std::mutex mutex;

        std::map<uint32_t, std::shared_ptr<sf::Sprite>> playerSprites;

        uint32_t remainingAnimTicks = 0;
        uint32_t prevAnim           = Entities::EntityPlayer::ANIM_BASE_STILL_FRONT;
        uint32_t prevPrevAnim       = Entities::EntityPlayer::ANIM_BASE_STILL_FRONT;
        uint32_t currentAnim        = Entities::EntityPlayer::ANIM_BASE_STILL_FRONT;

        PlayerInputs currentInputs;

        // Номер (sequence number) последнего пакета UpdatePlayerInputs,
        // который УЖЕ УЧЁЛ И ОБРАБОТАЛ (а не просто получил!) сервер.
        // Используется для прогнозирования передвижений (для controlled).
        std::atomic<uint32_t> lastSrvProcInputs = 0;

        void prepareSprites();

        void updatePlayerInputs();

        void updateAnimation();

        void saveInputsSnapshot(PlayerInputs inputsSnapshot);

        PlayerStateSnapshot takeStateSnapshot() const;

        void applyStateSnapshot(const PlayerStateSnapshot& playerState);

    public:
        uint32_t     playerId;
        std::wstring name;
        uint32_t     character;

        std::deque<PlayerInputs> recentInputsSnapshots;

        EntityPlayer(id_type entityId, uint32_t playerId,
                     const std::wstring& name, uint32_t character);

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   События Drawable
        ///////////////////////////////////////////////////////////////////////////////////////////

        void keyPressed(const sf::Event::KeyEvent &event) override;

        void update() override;

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Сеттеры (скорее даже "апдейтеры")
        ///////////////////////////////////////////////////////////////////////////////////////////

        void setLastSrvProcInputs(uint32_t ack);

        void setPosition(float newX, float newY, float newFaceAngle) override; // прогноз передвижения

        void turnSprite(const std::shared_ptr<sf::Sprite>& sprite, float newFaceAngle);

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Игровые события
        ///////////////////////////////////////////////////////////////////////////////////////////

        void positionChanged(float oldX, float oldY, float newX, float newY) override;

        void rotationChanged(float oldFaceAngle, float newFaceAngle) override;
    };

}

