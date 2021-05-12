#define SPRITE_WIDTH 128.0f


#include "EntityPlayer.hpp"
#include "../Game.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void EntityPlayer::prepareSprites() {
        // Стоит на месте. Повёрнут лицом к игроку (пользователю перед экраном).
        stillFrontSprite = std::make_shared<sf::Sprite>();

        stillFrontSprite->setPosition(x, y);
        stillFrontSprite->setTexture(*Game::instance().getTextures()->characters[character - 1]);

        sf::FloatRect baseBounds = stillFrontSprite->getGlobalBounds();
        stillFrontSprite->setScale( // устанавливаем нужный нам размер спрайта (из 16x32 --> WxH)
                spriteWidth  / baseBounds.width,
                spriteHeight / baseBounds.height
        );
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    EntityPlayer::EntityPlayer(id_type entityId,
                               const std::wstring& name, uint32_t character)
                               : LivingEntity(entityId) {
        this->name = name;
        this->character = character;

        spriteWidth  = SPRITE_WIDTH * renderScale;
        spriteHeight = 2.0f * spriteWidth;

        prepareSprites();
    }

    void EntityPlayer::keyPressed(const sf::Event::KeyEvent& event) {
        LivingEntity::keyPressed(event);
    }

    void EntityPlayer::update() {
        LivingEntity::update();
    }

}
