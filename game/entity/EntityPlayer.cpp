#define WALK_ANIMATION_SWITCH_DELAY_TICKS 2


#include "EntityPlayer.hpp"
#include "../Game.hpp"
#include "../../net/SequenceNumberMath.hpp"
#include "../util/RenderUtils.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void EntityPlayer::prepareSprites() {
        playerSprites[Entities::EntityPlayer::ANIM_BASE_STILL_FRONT]
                = createScaledSprite(Game::instance().getTextures()
                        ->characters[character][Entities::EntityPlayer::ANIM_BASE_STILL_FRONT]);

        playerSprites[Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_0]
                = createScaledSprite(Game::instance().getTextures()
                        ->characters[character][Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_0]);

        playerSprites[Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_1]
                = createScaledSprite(Game::instance().getTextures()
                        ->characters[character][Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_1]);

        playerSprites[Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_2]
                = createScaledSprite(Game::instance().getTextures()
                        ->characters[character][Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_2]);

        // Начальный спрайт.
        entitySprite = playerSprites[Entities::EntityPlayer::ANIM_BASE_STILL_FRONT];
    }

    void EntityPlayer::updatePlayerInputs() {
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

    void EntityPlayer::updateAnimation() {
        if (movedLastTick()) {
            if (remainingAnimTicks > 0)
                remainingAnimTicks--;
            else {
                if (currentAnim == Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_0)
                    currentAnim = prevPrevAnim == Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_1
                            ? Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_2
                            : Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_1;
                else
                    currentAnim = Entities::EntityPlayer::ANIM_BASE_WALK_RIGHT_0;

                remainingAnimTicks = WALK_ANIMATION_SWITCH_DELAY_TICKS;
            }
        } else {
            remainingAnimTicks = 0;
            currentAnim = Entities::EntityPlayer::ANIM_BASE_STILL_FRONT;
        }

        if (currentAnim != prevAnim) {
            prevPrevAnim = prevAnim;
            prevAnim     = currentAnim;
            entitySprite = playerSprites[currentAnim];
        }

        entitySprite->setPosition(x, y);
        turnSprite(entitySprite, faceAngle);
    }

    void EntityPlayer::saveInputsSnapshot(PlayerInputs inputsSnapshot) {
        if (inputsSnapshot.empty())
            // Нет смысла сохранять пустые снимки ввода на стороне клиента,
            // т.к. при повторном их применении (для чего мы эти снимки и
            // сохраняем) они не дадут совершенно никакого эффекта.
            return;

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

    bool operator ==(const PlayerStateSnapshot& a, const PlayerStateSnapshot& b) {
        return a.posX      == b.posX
            && a.posY      == b.posY
            && a.faceAngle == b.faceAngle;
    }

    bool operator !=(const PlayerStateSnapshot& a, const PlayerStateSnapshot& b) {
        return !(a == b);
    }

    PlayerInputs::PlayerInputs() = default;

    PlayerInputs::PlayerInputs(uint32_t localSequence, uint32_t inputsBitfield) {
        this->localSequence  = localSequence;
        this->inputsBitfield = inputsBitfield;
    }

    void PlayerInputs::reset() {
        inputsBitfield = 0;
    }

    void PlayerInputs::setMovingLeft(bool enable) {
        if    (enable) inputsBitfield |=  INPUT_MOVING_LEFT;
        else           inputsBitfield &= ~INPUT_MOVING_LEFT;
    }

    void PlayerInputs::setMovingRight(bool enable) {
        if    (enable) inputsBitfield |=  INPUT_MOVING_RIGHT;
        else           inputsBitfield &= ~INPUT_MOVING_RIGHT;
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
        if (movingLeft()) {
            playerState.posX -= Game::instance().getConfigs()->physics.playerBaseHorMs;
            playerState.faceAngle = 270.0f; // лицом влево
        }

        if (movingRight()) {
            playerState.posX += Game::instance().getConfigs()->physics.playerBaseHorMs;
            playerState.faceAngle = 90.0f; // лицом вправо
        }
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

        std::wcerr << L"Created EntityPlayer [" << name << L"]: " << entityId << std::endl;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   События Drawable
    ///////////////////////////////////////////////////////////////////////////////////////////

    void EntityPlayer::keyPressed(const sf::Event::KeyEvent& event) {
        LivingEntity::keyPressed(event);
    }

    void EntityPlayer::update() {
        std::unique_lock<std::mutex> lock(mutex);

        LivingEntity::update();

        if (isControlled)
            updatePlayerInputs();

        updateAnimation();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   Сеттеры (скорее даже "апдейтеры")
    ///////////////////////////////////////////////////////////////////////////////////////////

    void EntityPlayer::setLastSrvProcInputs(uint32_t ack) {
        std::unique_lock<std::mutex> lock(mutex);
        this->lastSrvProcInputs = ack;
    }

    void EntityPlayer::setPosition(float newX, float newY, float newFaceAngle) {
        std::unique_lock<std::mutex> lock(mutex);

        auto oldClientPlayerState = takeStateSnapshot();
        Entity::setPosition(newX, newY, newFaceAngle);

        if (isControlled) {
            // Сервер нас передвинул.
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

            if (fixedPlayerState == oldClientPlayerState)
                // Прогноз передвижения сработал корректно.
                applyStateSnapshot(fixedPlayerState);
            else {
                // Клиенту не удалось корректно спрогнозировать свою следующую позицию.
                // Скорее всего, на это повлияли какие-то факторы, известные серверу, но
                // не известные клиенту (по крайней мере неизвестные на момент предсказания).
                // Принимаем позицию, указанную сервером, без каких-либо локальных корректировок,
                // и на всякий случай очищаем локальную историю ввода (чтобы не попасть в адский
                // цикл бесконечных телепортаций (drag'ов).
                recentInputsSnapshots.clear();

                std::wcerr << L"Drag (prediction failure) - accepting server position: " << std::endl;
                std::wcerr << L"    x          : " << oldClientPlayerState.posX
                           << L" -> " << newX << L" -> " << fixedPlayerState.posX << std::endl;
                std::wcerr << L"    y          : " << oldClientPlayerState.posY
                           << L" -> " << newY << L" -> " << fixedPlayerState.posY << std::endl;
                std::wcerr << L"    face angle : " << oldClientPlayerState.faceAngle
                           << L" -> " << newFaceAngle << L" -> " << fixedPlayerState.faceAngle << std::endl;
                std::wcerr << std::endl;
            }
        }
    }

    void EntityPlayer::turnSprite(const std::shared_ptr<sf::Sprite>& sprite, float newFaceAngle) {
        if (newFaceAngle == 90.0f)
            // Разворачиваемся лицом вправо.
            RenderUtils::turnRight(entitySprite);
        else if (newFaceAngle == 270.0f)
            // Разворачиваемся лицом вправо.
            RenderUtils::turnLeft(entitySprite);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //   Игровые события
    ///////////////////////////////////////////////////////////////////////////////////////////

    void EntityPlayer::positionChanged(float oldX, float oldY, float newX, float newY) {
        if (!isControlled)
            return;

        // Фокусируем центр камеры (View) на центре модельки игрока.
        Game::instance().currentWorld()->focusCamera(
                newX + Game::instance().getConfigs()->physics.baseEntityPlayerW / 2.0f,
                newY + Game::instance().getConfigs()->physics.baseEntityPlayerH / 2.0f
        );
    }

    void EntityPlayer::rotationChanged(float oldFaceAngle, float newFaceAngle) {
        // TODO ?
    }

}
