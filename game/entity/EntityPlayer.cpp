#define BASE_PLAYER_TEXTURE_WIDTH 1.375f /* tiles */
#define BASE_PLAYER_TEXTURE_HEIGHT 2.125f /* tiles */


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
        scaleSprite(stillFrontSprite);

        //todo remove (update after setRotation instead)
        entitySprite = stillFrontSprite;
        //todo remove (update after setRotation instead)
    }

    void EntityPlayer::updatePlayerInputs() {
        if (!isControlled)
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
        this->playerId     = playerId;
        this->name         = name;
        this->character    = character;
        this->isControlled = playerId == Game::instance().getCurrentLobby()->ownPlayerId;

        spriteWidth  = BASE_PLAYER_TEXTURE_WIDTH;
        spriteHeight = BASE_PLAYER_TEXTURE_HEIGHT;

        prepareSprites();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   События Drawable
    ///////////////////////////////////////////////////////////////////////////////////////////

    void EntityPlayer::keyPressed(const sf::Event::KeyEvent& event) {
        LivingEntity::keyPressed(event);
    }

    void EntityPlayer::update() {
        LivingEntity::update();
        updatePlayerInputs();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   Игровые события
    ///////////////////////////////////////////////////////////////////////////////////////////

    void EntityPlayer::positionUpdated(float oldX, float oldY, float newX, float newY) {
        if (!isControlled)
            return;

        // Фокусируем центр камера (View) на центре модельки игрока.
        Game::instance().currentWorld()->focusCamera(
                newX + BASE_PLAYER_TEXTURE_WIDTH  / 2.0f,
                newY + BASE_PLAYER_TEXTURE_HEIGHT / 2.0f
        );
    }

    void EntityPlayer::rotationUpdated(float oldFaceAngle, float newFaceAngle) {
        //todo anything?
    }

}
