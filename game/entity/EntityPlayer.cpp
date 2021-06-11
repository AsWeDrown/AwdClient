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

    void EntityPlayer::updateMoveMechanics() {
        // Обновление текущего MoveMechanics для учёта графитации, пользовательского ввода и прочего.
        MoveMechanics newMoveMechanics;
        updateGravity(newMoveMechanics);
        updatePlayerInputs(newMoveMechanics);

        // Локальное применение прогроза передвижения (с учётом текущего MoveMechanics) и его сохранение.
        applyMoveMechanicsSnapshot(newMoveMechanics); // внутри делает "currentMoveMechanics = newMoveMechanics"
        saveMoveMechanicsSnapshot(newMoveMechanics);

        // Обновление пользовательского ввода (конкретно PlayerInputs) на сервере.
        Game::instance().getNetService()->updatePlayerInputs(
                currentMoveMechanics.playerInputs.inputsBitfield);
    }

    void EntityPlayer::updateGravity(MoveMechanics& moveMechanics) const {
        if (!moveMechanics.climbing) {
            float freeFallAcce = Game::instance().getConfigs()->physics.freeFallAcce;
            float midairTicks = currentMoveMechanics.midairTicks; // NOLINT(cppcoreguidelines-narrowing-conversions)
            moveMechanics.lastTickFallDistance = currentMoveMechanics.fallDistance;
            moveMechanics.fallDistance = (freeFallAcce * midairTicks * midairTicks) / 2.0f;
            moveMechanics.midairTicks = currentMoveMechanics.midairTicks + 1;
        }
    }

    void EntityPlayer::updatePlayerInputs(MoveMechanics& moveMechanics) const {
        if (Game::instance().isGameFocused()) {
            moveMechanics.playerInputs.setMovingLeft(
                    sf::Keyboard::isKeyPressed(sf::Keyboard::A)
                    || sf::Keyboard::isKeyPressed(sf::Keyboard::Left));

            moveMechanics.playerInputs.setMovingRight(
                    sf::Keyboard::isKeyPressed(sf::Keyboard::D)
                    || sf::Keyboard::isKeyPressed(sf::Keyboard::Right));

            moveMechanics.playerInputs.setMovingUp(
                    sf::Keyboard::isKeyPressed(sf::Keyboard::W)
                    || sf::Keyboard::isKeyPressed(sf::Keyboard::Up));

            moveMechanics.playerInputs.canonicalize();
        } else
            moveMechanics.playerInputs.reset();
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

    void EntityPlayer::applyMoveMechanicsSnapshot(const MoveMechanics& snapshot,
                                                  PosUpdateStrategy strategy) {
        auto terrainControls = Game::instance().currentWorld()->getTerrainControls();

        float newX         = posX;
        float newY         = posY;
        float newFaceAngle = faceAngle;

        /*
         * Гравитация.
         */
        if (snapshot.midairTicks > currentMoveMechanics.midairTicks)
            newY += snapshot.fallDistance - snapshot.lastTickFallDistance;

        /*
         * Пользовательский ввод.
         */
        if (snapshot.playerInputs.movingLeft()) {
            newX -= Game::instance().getConfigs()->physics.baseEntityPlayerMs;
            newFaceAngle = 270.0f; // лицом влево
        }

        if (snapshot.playerInputs.movingRight()) {
            newX += Game::instance().getConfigs()->physics.baseEntityPlayerMs;
            newFaceAngle = 90.0f; // лицом вправо
        }

        currentMoveMechanics.climbing = false; // сброс

        if (snapshot.playerInputs.movingUp()) {
            std::optional<TileBlock> intersectedLadder
                    = terrainControls->getFirstIntersectingTile(*this,[this](const TileBlock& tile)
                            { return tile.handler->isClimbableBy(*this); });

            if (intersectedLadder.has_value()) {
                // Игрок действительно находится на лестнице и может карабкаться.
                newY -= Game::instance().getConfigs()->physics.baseEntityPlayerClimbSpeed;
                currentMoveMechanics.climbing = true;

                // Сбрасываем гравитацию (вдруг игрок до этого был ей подвержен).
                currentMoveMechanics.midairTicks          =    0;
                currentMoveMechanics.lastTickFallDistance = 0.0f;
                currentMoveMechanics.fallDistance         = 0.0f;
            }
        }

        /*
         * Применяем новую позицию с учётом текущего состояния мира (препятствия вокруг и т.п.).
         */
        internalSetPosition(
                terrainControls->advanceTowardsXUntilTerrainCollision(*this, newX),
                terrainControls->advanceTowardsYUntilTerrainCollision(*this, newY),
                newFaceAngle,
                strategy
        );

        /*
         * Наконец, смотрим на фактические изменения состояния игрока и делаем соответствующие
         * обновления (например, "в этот тик игрок немного упал вниз и приземлился - в следующий
         * тик (при применении следующего снимка MoveMechanics) он уже будет стоять на земле").
         */
        currentMoveMechanics = snapshot;

        if (terrainControls->isOnGround(*this)) {
            currentMoveMechanics.midairTicks          =    0;
            currentMoveMechanics.lastTickFallDistance = 0.0f;
            currentMoveMechanics.fallDistance         = 0.0f;
        }
    }

    void EntityPlayer::saveMoveMechanicsSnapshot(const MoveMechanics& snapshot) {
        if (snapshot.empty())
            // Нет смысла сохранять пустые снимки ввода на стороне клиента,
            // т.к. при повторном их применении (для чего мы эти снимки и
            // сохраняем) они не дадут совершенно никакого эффекта. А вот
            // отправлять серверу их нужно - потому что сервер обрабатывает
            // строго по одному вводу из очереди получения каждый серверный тик.
            return;

        if (recentMoveMechanicsSnapshots.size() == Game::instance().getConfigs()->physics.maxLag)
            recentMoveMechanicsSnapshots.pop_front();

        recentMoveMechanicsSnapshots.emplace_back(
                Game::instance().getPacketManager()->getHandle()->getLocalSequenceNumber(),
                snapshot.playerInputs,
                snapshot.climbing,
                snapshot.midairTicks,
                snapshot.lastTickFallDistance,
                snapshot.fallDistance
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

    void PlayerInputs::setMovingUp(bool enable) {
        if    (enable) inputsBitfield |=  INPUT_MOVING_UP;
        else           inputsBitfield &= ~INPUT_MOVING_UP;
    }

    void PlayerInputs::canonicalize() {
        if (movingLeft() && movingRight()) {
            setMovingLeft(false);
            setMovingRight(false);
        }
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

    bool PlayerInputs::movingUp() const {
        return (inputsBitfield & INPUT_MOVING_UP) != 0;
    }

    MoveMechanics::MoveMechanics() = default;

    MoveMechanics::MoveMechanics(uint32_t localSequence, PlayerInputs playerInputs,
                                 bool climbing, uint32_t midairTicks, float lastTickFallDistance, float fallDistance) {
        this->localSequence = localSequence;
        this->playerInputs = playerInputs;
        this->climbing = climbing;
        this->midairTicks = midairTicks;
        this->lastTickFallDistance = lastTickFallDistance;
        this->fallDistance = fallDistance;
    }

    bool MoveMechanics::empty() const {
        return playerInputs.empty() && midairTicks == 0;
    }

    void MoveMechanics::reset() {
        playerInputs.reset();
        climbing = false;
        midairTicks = 0;
        lastTickFallDistance = 0.0f;
        fallDistance = 0.0f;
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
        std::unique_lock<std::mutex> lock(mutex);

        LivingEntity::update();

        if (isControlled)
            updateMoveMechanics();

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

        currentMoveMechanics.midairTicks          = midairTicks;
        currentMoveMechanics.lastTickFallDistance = lastTickFallDistance;
        currentMoveMechanics.fallDistance         = fallDistance;
    }

    void EntityPlayer::setPosition(float newX, float newY, float newFaceAngle) {
        if (!isControlled) {
            // Интерполируем.
            Entity::setPosition(newX, newY, newFaceAngle);
            return;
        }

        std::unique_lock<std::mutex> lock(mutex);
        bool isInitialMove = posX == 0.0f && posY == 0.0f; // сервер нас только что отправил на позицию спавна

        if (isInitialMove) {
            // Не учитываем никакое прогнозирование до спавна
            internalSetPosition(newX, newY, newFaceAngle);
            currentMoveMechanics.reset();
            recentMoveMechanicsSnapshots.clear();

            return;
        }

        auto oldClientPlayerState = takeStateSnapshot();
        internalSetPosition(newX, newY, newFaceAngle, PosUpdateStrategy::SILENT);

        if (isControlled) {
            // Сервер нас передвинул.
            uint32_t ack = this->lastSrvProcInputs;

            // Стираем из списка недавно применённого ввода тот ввод, который
            // сервер уже учёл в этой, т.е. полученной только что, позиции.
            recentMoveMechanicsSnapshots.erase(std::remove_if(
                    recentMoveMechanicsSnapshots.begin(), recentMoveMechanicsSnapshots.end(),
                    [ack](const MoveMechanics& snapshot) {
                        return !net::SequenceNumberMath::isMoreRecent(snapshot.localSequence, ack);
                    }), recentMoveMechanicsSnapshots.end()
            );

            // Заново применяем тот ввод, который сервер ещё не успел учесть.
            for (const auto& snapshot : recentMoveMechanicsSnapshots)
                applyMoveMechanicsSnapshot(snapshot, PosUpdateStrategy::SILENT);

            internalSetPosition(posX, posY, faceAngle, PosUpdateStrategy::FORCED);
            auto correctedPlayerState = takeStateSnapshot();

            if (correctedPlayerState != oldClientPlayerState) {
                // Клиенту не удалось корректно спрогнозировать свою следующую позицию.
                // Скорее всего, на это повлияли какие-то факторы, известные серверу, но
                // не известные клиенту (по крайней мере неизвестные на момент предсказания).
                // Принимаем позицию, указанную сервером, без каких-либо локальных корректировок,
                // и на всякий случай очищаем локальную историю ввода (чтобы не попасть в адский
                // цикл бесконечных телепортаций (drag'ов).
                uint32_t lag = recentMoveMechanicsSnapshots.empty() ? 0 : net::SequenceNumberMath::
                        subtract(recentMoveMechanicsSnapshots[
                                 recentMoveMechanicsSnapshots.size() - 1].localSequence, ack);

                internalSetPosition(newX, newY, newFaceAngle);
                recentMoveMechanicsSnapshots.clear();

                std::wcerr << L"Drag (prediction failure, lag=" << lag << L") - accepting server position: " << std::endl;
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
