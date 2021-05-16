#include "DespawnEntityListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/play/PlayScreen.hpp"

namespace awd::game {

    void DespawnEntityListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        auto packet = std::dynamic_pointer_cast<net::DespawnEntity>(basePacket);
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (Game::instance().getCurrentState() == GameState::PLAY) {
            if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen)) {
                std::wcout << L"Despawned entity " << packet->entity_id() << std::endl;
                playScreen->getWorld()->removeEntity(packet->entity_id());
            }
        }
    }

}
