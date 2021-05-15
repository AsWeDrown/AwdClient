#define BASE_PLAYER_TEXTURE_WIDTH 22.0f
#define BASE_PLAYER_TEXTURE_HEIGHT 34.0f


#include "EntityPlayer.hpp"
#include "../Game.hpp"
#include "Entities.hpp"

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
        stillFrontSprite->setScale( // устанавливаем нужный нам размер спрайта
                spriteWidth  / baseBounds.width,
                spriteHeight / baseBounds.height
        );

        //todo remove
        entitySprite = stillFrontSprite;
        //todo remove
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    EntityPlayer::EntityPlayer(id_type entityId,
                               const std::wstring& name, uint32_t character)
                               : LivingEntity(Entities::EntityPlayer::TYPE, entityId) {
        this->name = name;
        this->character = character;

        spriteWidth  = BASE_PLAYER_TEXTURE_WIDTH;
        spriteHeight = BASE_PLAYER_TEXTURE_HEIGHT;

        prepareSprites();
    }

    void EntityPlayer::keyPressed(const sf::Event::KeyEvent& event) {
        LivingEntity::keyPressed(event);
    }

    void EntityPlayer::update() {
        LivingEntity::update();
    }

}
