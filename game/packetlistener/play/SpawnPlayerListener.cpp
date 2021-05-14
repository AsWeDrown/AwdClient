#include "SpawnPlayerListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/play/PlayScreen.hpp"
#include "../../entity/EntityPlayer.hpp"

namespace awd::game {

    void SpawnPlayerListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        auto packet = std::dynamic_pointer_cast<net::SpawnPlayer>(basePacket);
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (Game::instance().getCurrentState() == GameState::PLAY) {
            if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen)) {
                std::wcout << L"Player " << packet->player_id() << L" spawned with entity ID "
                           << packet->entity_id() << std::endl;

                auto lobby  = Game::instance().getCurrentLobby();
                auto player = std::make_shared<EntityPlayer>(
                        packet->entity_id(),
                        lobby->playerNames[packet->player_id()],
                        lobby->playerCharacters[packet->player_id()]
                );

                playScreen->getWorld()->addEntity(player);
            }
        }
    }

}
