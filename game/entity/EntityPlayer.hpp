#pragma once


#include "LivingEntity.hpp"

namespace awd::game {

    class PlayerInputs {
    public:
        static const uint32_t BIT_MOVING_LEFT  = 0b1,
                              BIT_MOVING_RIGHT = 0b10;

        bool movingLeft  = false,
             movingRight = false;
    };

    bool operator ==(const PlayerInputs& a, const PlayerInputs& b);
    bool operator !=(const PlayerInputs& a, const PlayerInputs& b);

    class EntityPlayer : public LivingEntity {
    private:
        // Стоит на месте. Повёрнут лицом к игроку (пользователю перед экраном).
        std::shared_ptr<sf::Sprite> stillFrontSprite;

        void prepareSprites();

        void updatePlayerInputs();

    public:
        uint32_t     playerId;
        std::wstring name;
        uint32_t     character;

        std::shared_ptr<PlayerInputs> playerInputs = std::make_shared<PlayerInputs>();

        EntityPlayer(id_type entityId, uint32_t playerId,
                     const std::wstring& name, uint32_t character);

        void keyPressed(const sf::Event::KeyEvent &event) override;
        void update() override;
    };

}

