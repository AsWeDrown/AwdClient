#define BASE_PLAYER_TEXTURE_WIDTH 1.375f /* tiles */
#define BASE_PLAYER_TEXTURE_HEIGHT 2.125f /* tiles */

#define MAX_RECENT_INPUT_SNAPSHOTS 5


#include "EntityPlayer.hpp"
#include "../Game.hpp"
#include "Entities.hpp"
#include "../../net/SequenceNumberMath.hpp"

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

    void EntityPlayer::updatePlayerActions() {
        PlayerActions inputs;

        inputs.setMoveLeft (sf::Keyboard::isKeyPressed(sf::Keyboard::A));
        inputs.setMoveRight(sf::Keyboard::isKeyPressed(sf::Keyboard::D));

        if (!inputs.empty()) {
            std::unique_lock<std::mutex> lock(mutex);

            // Прогноз передвижения.
            auto updatedPlayerState = takeStateSnapshot();
            inputs.apply(updatedPlayerState);
            applyStateSnapshot(updatedPlayerState);
            saveInputsSnapshot(inputs);

            // Отправка информации о совершение действий серверу.
            Game::instance().getNetService()->playerActions(inputs.actionsBitfield);
        }
    }

    void EntityPlayer::saveInputsSnapshot(PlayerActions inputsSnapshot) {
        if (recentInputsSnapshots.size() == MAX_RECENT_INPUT_SNAPSHOTS)
            recentInputsSnapshots.pop_front();

        recentInputsSnapshots.emplace_back(
                Game::instance().getPacketManager()->getHandle()->getLocalSequenceNumber(),
                inputsSnapshot.actionsBitfield
        );
    }

    PlayerStateSnapshot EntityPlayer::takeStateSnapshot() const {
        PlayerStateSnapshot playerState;

        playerState.posX      = posX;
        playerState.posY      = posY;
        playerState.faceAngle = faceAngle;

        return playerState;
    }

    void EntityPlayer::applyStateSnapshot(const PlayerStateSnapshot& playerState) {
        //todo rotation
        internalSetPosition(
                playerState.posX,
                playerState.posY
        );
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    PlayerActions::PlayerActions() = default;

    PlayerActions::PlayerActions(uint32_t localSequence, uint32_t actionsBitfield) {
        this->localSequence   = localSequence;
        this->actionsBitfield = actionsBitfield;
    }

    void PlayerActions::setMoveLeft(bool enableAction) {
        if (enableAction) actionsBitfield |=  ACTION_MOVE_LEFT;
        else              actionsBitfield &= ~ACTION_MOVE_LEFT;
    }

    void PlayerActions::setMoveRight(bool enableAction) {
        if (enableAction) actionsBitfield |=  ACTION_MOVE_RIGHT;
        else              actionsBitfield &= ~ACTION_MOVE_RIGHT;
    }

    bool PlayerActions::empty() const {
        return actionsBitfield == 0;
    }

    bool PlayerActions::moveLeft() const {
        return (actionsBitfield & ACTION_MOVE_LEFT) != 0;
    }

    bool PlayerActions::moveRight() const {
        return (actionsBitfield & ACTION_MOVE_RIGHT) != 0;
    }

    void PlayerActions::apply(PlayerStateSnapshot& playerState) const {
        if (moveLeft())
            playerState.posX -= Game::instance().getConfigs()->physics.playerBaseHorizontalMoveSpeed;

        if (moveRight())
            playerState.posX += Game::instance().getConfigs()->physics.playerBaseHorizontalMoveSpeed;
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

        if (isControlled)
            updatePlayerActions();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   Сеттеры (скорее даже "апдейтеры")
    ///////////////////////////////////////////////////////////////////////////////////////////

    void EntityPlayer::setPosition(float newX, float newY) {
        float oldCltX = posX;

        Entity::setPosition(newX, newY);

        if (isControlled) {
            uint32_t newestAck = Game::instance()
                    .getPacketManager()->getHandle()->getNewestAck();

            std::unique_lock<std::mutex> lock(mutex);

            recentInputsSnapshots.erase(std::remove_if(
                    recentInputsSnapshots.begin(), recentInputsSnapshots.end(),
                    [newestAck](PlayerActions snapshot) {
                        return !net::SequenceNumberMath::isMoreRecent(snapshot.localSequence, newestAck);
                    }), recentInputsSnapshots.end()
            );

            auto fixedPlayerState = takeStateSnapshot();

            for (const auto& snapshot : recentInputsSnapshots)
                snapshot.apply(fixedPlayerState);

            applyStateSnapshot(fixedPlayerState);

            if (oldCltX != posX)
                std::wcerr << L"[DRAG] deltaX = " << (posX-oldCltX) << std::endl;
        }
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
