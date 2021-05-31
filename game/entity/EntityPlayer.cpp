#define WALK_ANIMATION_SWITCH_DELAY_TICKS 3


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

        // Создание прогноза передвижения.
        DeltaPosition deltaPosSnapshot;

        applyGravityForce(deltaPosSnapshot);
        applyPlayerInputs(deltaPosSnapshot);

        // Локальное применение прогроза передвижения.
        applyDeltaPosSnapshot(deltaPosSnapshot);
        saveDeltaPosSnapshot(deltaPosSnapshot);

        // Обновление пользовательского ввода на сервере.
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

    void EntityPlayer::applyGravityForce(DeltaPosition& deltaPosSnapshot) {
        float freeFallAcce = Game::instance().getConfigs()->physics.freeFallAcce;
        fallDistance = (freeFallAcce * midairTicks * midairTicks) / 2.0f; // NOLINT(cppcoreguidelines-narrowing-conversions)
        deltaPosSnapshot.deltaY = fallDistance - lastTickFallDistance;
        lastTickFallDistance = fallDistance;
    }

    void EntityPlayer::applyPlayerInputs(DeltaPosition& deltaPosSnapshot) {
        float moveSpeed = Game::instance().getConfigs()->physics.baseEntityPlayerMs;

        if (currentInputs.movingLeft()) {
            deltaPosSnapshot.deltaX -= moveSpeed;
            deltaPosSnapshot.faceAngle = 270.0f; // лицом влево
        }

        if (currentInputs.movingRight()) {
            deltaPosSnapshot.deltaX += moveSpeed;
            deltaPosSnapshot.faceAngle = 90.0f; // лицом вправо
        }
    }

    void EntityPlayer::applyDeltaPosSnapshot(const DeltaPosition& deltaPosSnapshot, bool silent) {
        auto terrainControls = Game::instance().currentWorld()->getTerrainControls();

        float destX = posX + deltaPosSnapshot.deltaX;
        float destY = posY + deltaPosSnapshot.deltaY;

        internalSetPosition(
                terrainControls->advanceTowardsXUntilTerrainCollision(*this, destX),
                terrainControls->advanceTowardsYUntilTerrainCollision(*this, destY),
                deltaPosSnapshot.faceAngle == -1.0f ? this->faceAngle : deltaPosSnapshot.faceAngle,
                silent
        );

        if (terrainControls->isOnGround(*this)) {
            midairTicks          = 0;
            lastTickFallDistance = 0.0f;
            fallDistance         = 0.0f;
        } else
            midairTicks++;
    }

    void EntityPlayer::saveDeltaPosSnapshot(const DeltaPosition& deltaPosSnapshot) {
        if (deltaPosSnapshot.empty())
            // Нет смысла сохранять пустые снимки ввода на стороне клиента,
            // т.к. при повторном их применении (для чего мы эти снимки и
            // сохраняем) они не дадут совершенно никакого эффекта. А вот
            // отправлять серверу их нужно - потому что сервер обрабатывает
            // строго по одному вводу из очереди получения каждый серверный тик.
            return;

        if (recentDeltaPosSnapshots.size() == Game::instance().getConfigs()->physics.maxLag)
            recentDeltaPosSnapshots.pop_front();

        recentDeltaPosSnapshots.emplace_back(
                Game::instance().getPacketManager()->getHandle()->getLocalSequenceNumber(),
                deltaPosSnapshot.deltaX,
                deltaPosSnapshot.deltaY,
                deltaPosSnapshot.faceAngle
        );
    }

    PlayerStateSnapshot EntityPlayer::takeStateSnapshot() const {
        PlayerStateSnapshot playerState;

        playerState.posX      = posX;
        playerState.posY      = posY;
        playerState.faceAngle = faceAngle;

        return playerState;
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

    DeltaPosition::DeltaPosition() = default;

    DeltaPosition::DeltaPosition(uint32_t localSequence, float deltaX, float deltaY, float faceAngle) {
        this->localSequence = localSequence;
        this->deltaX        = deltaX;
        this->deltaY        = deltaY;
        this->faceAngle     = faceAngle;
    }

    bool DeltaPosition::empty() const {
        return deltaX == 0.0f && deltaY == 0.0f;
    }

    EntityPlayer::EntityPlayer(id_type entityId, uint32_t playerId,
                               const std::wstring& name, uint32_t character)
                               : FallableLivingEntity(Entities::EntityPlayer::TYPE, entityId) {
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

    void EntityPlayer::setGravityData(uint32_t midairTicks,
                                      float lastTickFallDistance, float fallDistance) {
        std::unique_lock<std::mutex> lock(mutex);

        this->midairTicks          = midairTicks;
        this->lastTickFallDistance = lastTickFallDistance;
        this->fallDistance         = fallDistance;
    }

    void EntityPlayer::setPosition(float newX, float newY, float newFaceAngle) {
        if (!isControlled) {
            Entity::setPosition(newX, newY, newFaceAngle);
            return;
        }

        bool isInitialMove = posX == 0.0f && posY == 0.0f; // сервер нас только что отправил на позицию спавна

        if (isInitialMove) {
            internalSetPosition(newX, newY, newFaceAngle); // не учитываем никакое прогнозирование до спавна
            return;
        }

        std::unique_lock<std::mutex> lock(mutex);

        // Здесь и далее все "промежуточные" перемещения осуществляем с silent=true.
        // Подробнее - см. объявление (declaration) метода Entity#internalSetPosition.
        auto oldClientPlayerState = takeStateSnapshot();
        internalSetPosition(newX, newY, newFaceAngle, true);

        if (isControlled) {
            // Сервер нас передвинул.
            uint32_t ack = this->lastSrvProcInputs;

            // Стираем из списка недавно применённого ввода тот ввод, который
            // сервер уже учёл в этой, т.е. полученной только что, позиции.
            recentDeltaPosSnapshots.erase(std::remove_if(
                    recentDeltaPosSnapshots.begin(), recentDeltaPosSnapshots.end(),
                    [ack](DeltaPosition snapshot) {
                        return !net::SequenceNumberMath::isMoreRecent(snapshot.localSequence, ack);
                    }), recentDeltaPosSnapshots.end()
            );

            // Заново применяем тот ввод, который сервер ещё не успел учесть.
            uint32_t lastSnapshotIndex = recentDeltaPosSnapshots.size() - 1; 

            for (int i = 0; i < recentDeltaPosSnapshots.size(); i++)
                applyDeltaPosSnapshot(recentDeltaPosSnapshots[i], i != lastSnapshotIndex);

            auto correctedPlayerState = takeStateSnapshot();

            if (correctedPlayerState != oldClientPlayerState) {
                // Клиенту не удалось корректно спрогнозировать свою следующую позицию.
                // Скорее всего, на это повлияли какие-то факторы, известные серверу, но
                // не известные клиенту (по крайней мере неизвестные на момент предсказания).
                // Принимаем позицию, указанную сервером, без каких-либо локальных корректировок,
                // и на всякий случай очищаем локальную историю ввода (чтобы не попасть в адский
                // цикл бесконечных телепортаций (drag'ов).
                internalSetPosition(newX, newY, newFaceAngle);
                recentDeltaPosSnapshots.clear();

                std::wcerr << L"Drag (prediction failure) - accepting server position: " << std::endl;
                std::wcerr << L"    x          : " << oldClientPlayerState.posX
                           << L" -> " << newX << L" -> " << correctedPlayerState.posX << std::endl;
                std::wcerr << L"    y          : " << oldClientPlayerState.posY
                           << L" -> " << newY << L" -> " << correctedPlayerState.posY << std::endl;
                std::wcerr << L"    face angle : " << oldClientPlayerState.faceAngle
                           << L" -> " << newFaceAngle << L" -> " << correctedPlayerState.faceAngle << std::endl;
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
                newX + spriteWidth  / 2.0f,
                newY + spriteHeight / 2.0f
        );
    }

    void EntityPlayer::rotationChanged(float oldFaceAngle, float newFaceAngle) {
        // TODO ?
    }

}
