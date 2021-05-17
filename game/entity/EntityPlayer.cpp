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

        //todo remove (update after setRotation instead)
        entitySprite = stillFrontSprite;
        //todo remove (update after setRotation instead)
    }

    void EntityPlayer::updatePlayerInputs() {
        if (playerId != Game::instance().getCurrentLobby()->ownPlayerId)
            return;

        auto newPlayerInputs = std::make_shared<PlayerInputs>();

        newPlayerInputs->movingLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        newPlayerInputs->movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

        if (*playerInputs != *newPlayerInputs) {
            playerInputs = newPlayerInputs;
            uint32_t inputsBitfield = 0;

            if (playerInputs->movingLeft ) inputsBitfield |= PlayerInputs::BIT_MOVING_LEFT ;
            if (playerInputs->movingRight) inputsBitfield |= PlayerInputs::BIT_MOVING_RIGHT;

            Game::instance().getNetService()->updatePlayerInputs(inputsBitfield);
        }
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    bool operator ==(const PlayerInputs& a, const PlayerInputs& b) {
        return a.movingLeft  == b.movingLeft
            && a.movingRight == b.movingRight;
    }

    bool operator !=(const PlayerInputs& a, const PlayerInputs& b) {
        return !(a == b);
    }

    EntityPlayer::EntityPlayer(id_type entityId, uint32_t playerId,
                               const std::wstring& name, uint32_t character)
                               : LivingEntity(Entities::EntityPlayer::TYPE, entityId) {
        this->playerId  = playerId;
        this->name      = name;
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
        updatePlayerInputs();
    }

}
