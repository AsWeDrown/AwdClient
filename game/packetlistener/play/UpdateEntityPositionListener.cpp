#include "UpdateEntityPositionListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/play/PlayScreen.hpp"

namespace awd::game {

    void UpdateEntityPositionListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        auto packet = std::dynamic_pointer_cast<net::UpdateEntityPosition>(basePacket);
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (Game::instance().getCurrentState() == GameState::PLAY) {
            if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen)) {
                auto entity = playScreen->getWorld()->getEntityById(packet->entity_id());

                if (entity != nullptr) {
                    entity->setPosition(packet->pos_x(), packet->pos_y());
                    entity->setRotation(packet->face_angle());
                } else
                    std::wcerr << L"Ignoring position update "
                                  L"of an unknown entity: ID = " << packet->entity_id() << std::endl;
            }
        }
    }

}
