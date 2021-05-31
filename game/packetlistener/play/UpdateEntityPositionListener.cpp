#include "UpdateEntityPositionListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/play/PlayScreen.hpp"
#include "../../entity/EntityPlayer.hpp"

namespace awd::game {

    void UpdateEntityPositionListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        auto packet = std::dynamic_pointer_cast<net::UpdateEntityPosition>(packetData->getPacket());
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (Game::instance().getCurrentState() == GameState::PLAY) {
            if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen)) {
                auto entity = playScreen->getWorld()->getEntityById(packet->entity_id());

                if (entity != nullptr) {
                    if (entity->isControlledPlayer()) {
                        // Нужно для корректного прогнозирования передвижения. Здесь ack - это
                        // номер последнего УЧТЁННОГО (не просто полученного!) сервером на данный
                        // момент пакета UpdatePlayerInputs (т.е. "ack" тут не в "обычном" смысле).
                        auto player = std::dynamic_pointer_cast<game::EntityPlayer>(entity);
                        player->setLastSrvProcInputs(packetData->getAck());
                        player->setGravityData(packet->midair_ticks(),
                                               packet->last_tick_fall_distance(), packet->fall_distance());
                    }

                    entity->setPosition(packet->pos_x(), packet->pos_y(), packet->face_angle());
                } else
                    std::wcerr << L"Ignoring position update "
                                  L"of an unknown entity: ID = " << packet->entity_id() << std::endl;
            }
        }
    }

}
