#pragma once


#include "LivingEntity.hpp"

namespace awd::game {

//    class PlayerInputs {
//    public:
//        static const uint32_t BIT_MOVING_LEFT  = 0b1,
//                              BIT_MOVING_RIGHT = 0b10;
//
//        uint32_t localSequence = 0;
//
//        bool movingLeft  = false,
//             movingRight = false;
//    };
//
//    bool operator ==(const PlayerInputs& a, const PlayerInputs& b);
//    bool operator !=(const PlayerInputs& a, const PlayerInputs& b);

    class PlayerStateSnapshot {
    public:
        float posX      = 0.0f,
              posY      = 0.0f,
              faceAngle = 0.0f;
    };

    class PlayerActions {
    public:
        static const uint32_t ACTION_MOVE_LEFT  = 0b1;
        static const uint32_t ACTION_MOVE_RIGHT = 0b10;

        uint32_t localSequence   = 0,
                 actionsBitfield = 0;

        PlayerActions();
        PlayerActions(uint32_t localSequence, uint32_t actionsBitfield);

        void setMoveLeft (bool enableAction);
        void setMoveRight(bool enableAction);

        bool empty() const;

        bool moveLeft () const;
        bool moveRight() const;

        void apply(PlayerStateSnapshot& playerState) const;
    };

    class EntityPlayer : public LivingEntity {
    private:
        std::mutex mutex;

        // Стоит на месте. Повёрнут лицом к игроку (пользователю перед экраном).
        std::shared_ptr<sf::Sprite> stillFrontSprite;

        void prepareSprites();

        void updatePlayerActions();

        //void updatePlayerInputs();

        void saveInputsSnapshot(PlayerActions inputsSnapshot);

        PlayerStateSnapshot takeStateSnapshot() const;

        void applyStateSnapshot(const PlayerStateSnapshot& playerState);

    public:
        uint32_t     playerId;
        std::wstring name;
        uint32_t     character;

        std::deque<PlayerActions> recentInputsSnapshots;

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

        void setPosition(float newX, float newY) override;

        ///////////////////////////////////////////////////////////////////////////////////////////
        //   Игровые события
        ///////////////////////////////////////////////////////////////////////////////////////////

        void positionUpdated(float oldX, float oldY, float newX, float newY) override;

        void rotationUpdated(float oldFaceAngle, float newFaceAngle) override;
    };

}

