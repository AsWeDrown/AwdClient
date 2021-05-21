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

    void EntityPlayer::updatePlayerInputs() {
        std::unique_lock<std::mutex> lock(mutex);

        if (Game::instance().isGameFocused()) {
            currentInputs.setMovingLeft (sf::Keyboard::isKeyPressed(sf::Keyboard::A));
            currentInputs.setMovingRight(sf::Keyboard::isKeyPressed(sf::Keyboard::D));
        } else
            currentInputs.reset();

        // Прогноз передвижения.
        auto updatedPlayerState = takeStateSnapshot();
        currentInputs.apply(updatedPlayerState);
        applyStateSnapshot(updatedPlayerState);
        saveInputsSnapshot(currentInputs);

        // Обновление ввода на сервере.
        Game::instance().getNetService()->updatePlayerInputs(currentInputs.inputsBitfield);
    }

    void EntityPlayer::saveInputsSnapshot(PlayerInputs inputsSnapshot) {
        if (recentInputsSnapshots.size() == Game::instance().getConfigs()->physics.maxLag)
            recentInputsSnapshots.pop_front();

        recentInputsSnapshots.emplace_back(
                Game::instance().getPacketManager()->getHandle()->getLocalSequenceNumber(),
                inputsSnapshot.inputsBitfield
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
        internalSetPosition(
                playerState.posX,
                playerState.posY,
                playerState.faceAngle
        );
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    PlayerInputs::PlayerInputs() = default;

    PlayerInputs::PlayerInputs(uint32_t localSequence, uint32_t inputsBitfield) {
        this->localSequence  = localSequence;
        this->inputsBitfield = inputsBitfield;
    }

    void PlayerInputs::reset() {
        inputsBitfield = 0;
    }

    void PlayerInputs::setMovingLeft(bool enable) {
        if (enable) inputsBitfield |=  INPUT_MOVING_LEFT;
        else        inputsBitfield &= ~INPUT_MOVING_LEFT;
    }

    void PlayerInputs::setMovingRight(bool enable) {
        if (enable) inputsBitfield |=  INPUT_MOVING_RIGHT;
        else        inputsBitfield &= ~INPUT_MOVING_RIGHT;
    }

    bool PlayerInputs::empty() const {
        return inputsBitfield == 0;
    }

    bool PlayerInputs::movingLeft() const {
        return (inputsBitfield & INPUT_MOVING_LEFT) != 0;
    }

    bool PlayerInputs::movingRight() const {
        return (inputsBitfield & INPUT_MOVING_RIGHT) != 0;
    }

    void PlayerInputs::apply(PlayerStateSnapshot& playerState) const {
        if (movingLeft())
            playerState.posX -= Game::instance().getConfigs()->physics.playerBaseHorMs;

        if (movingRight())
            playerState.posX += Game::instance().getConfigs()->physics.playerBaseHorMs;
    }

    EntityPlayer::EntityPlayer(id_type entityId, uint32_t playerId,
                               const std::wstring& name, uint32_t character)
                               : LivingEntity(Entities::EntityPlayer::TYPE, entityId) {
        this->playerId     = playerId;
        this->name         = name;
        this->character    = character;
        this->isControlled = playerId == Game::instance().getCurrentLobby()->ownPlayerId;

        spriteWidth  = Game::instance().getConfigs()->physics.baseEntityPlayerW;
        spriteHeight = Game::instance().getConfigs()->physics.baseEntityPlayerH;

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
            updatePlayerInputs();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   Сеттеры (скорее даже "апдейтеры")
    ///////////////////////////////////////////////////////////////////////////////////////////

    void EntityPlayer::setLastSrvProcInputs(uint32_t ack) {
        std::unique_lock<std::mutex> lock(mutex);
        this->lastSrvProcInputs = ack;
    }

    void EntityPlayer::setPosition(float newX, float newY, float newFaceAngle) {
        Entity::setPosition(newX, newY, newFaceAngle);

        if (isControlled) {
            // Сервер нас передвинул.
            std::unique_lock<std::mutex> lock(mutex);
            uint32_t ack = this->lastSrvProcInputs;

            // Стираем из списка недавно применённого ввода тот ввод, который
            // сервер уже учёл в этой, т.е. полученной только что, позиции.
            recentInputsSnapshots.erase(std::remove_if(
                    recentInputsSnapshots.begin(), recentInputsSnapshots.end(),
                    [ack](PlayerInputs snapshot) {
                        return !net::SequenceNumberMath::isMoreRecent(snapshot.localSequence, ack);
                    }), recentInputsSnapshots.end()
            );

            // Заново применяем тот ввод, который сервер ещё не успел учесть.
            auto fixedPlayerState = takeStateSnapshot();

            for (const auto& snapshot : recentInputsSnapshots)
                snapshot.apply(fixedPlayerState);

            applyStateSnapshot(fixedPlayerState);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   Игровые события
    ///////////////////////////////////////////////////////////////////////////////////////////

    void EntityPlayer::positionChanged(float oldX, float oldY, float newX, float newY) {
        if (!isControlled)
            return;

        // Фокусируем центр камера (View) на центре модельки игрока.
        Game::instance().currentWorld()->focusCamera(
                newX + Game::instance().getConfigs()->physics.baseEntityPlayerW / 2.0f,
                newY + Game::instance().getConfigs()->physics.baseEntityPlayerH / 2.0f
        );
    }

    void EntityPlayer::rotationChanged(float oldFaceAngle, float newFaceAngle) {
        //todo anything?
    }

}
