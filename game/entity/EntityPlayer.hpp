#pragma once


#include "LivingEntity.hpp"

namespace awd::game {

    class EntityPlayer : public LivingEntity {
    private:
        // Стоит на месте. Повёрнут лицом к игроку (пользователю перед экраном).
        std::shared_ptr<sf::Sprite> stillFrontSprite;

        void prepareSprites();

    public:
        std::wstring name;
        uint32_t character;

        EntityPlayer(id_type entityId,
                     const std::wstring& name, uint32_t character);

        void keyPressed(const sf::Event::KeyEvent &event) override;
        void update() override;
    };

}

