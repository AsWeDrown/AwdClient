#include "SpawnEntityListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/play/PlayScreen.hpp"
#include "../../entity/EntityPlayer.hpp"
#include "../../entity/Entities.hpp"

namespace awd::game {

    void SpawnEntityListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        auto packet = std::dynamic_pointer_cast<net::SpawnEntity>(basePacket);
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (Game::instance().getCurrentState() == GameState::PLAY) {
            if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen)) {
                uint32_t entityType = packet->entity_type();
                uint32_t entityId   = packet->entity_id();
                auto     entityData = packet->entity_data(); // map<string, string>

                switch (entityType) {
                    case Entities::EntityPlayer::TYPE: {
                        uint32_t playerId = std::stoi(entityData["player_id"]);

                        std::wcout << L"Player " << playerId << L" spawned with entity ID "
                                   << entityId << std::endl;

                        auto lobby = Game::instance().getCurrentLobby();
                        auto player = std::make_shared<EntityPlayer>(
                                packet->entity_id(),
                                playerId,
                                lobby->playerNames[playerId],
                                lobby->playerCharacters[playerId]
                        );

                        playScreen->getWorld()->addEntity(player);

                        break;
                    }

                    default:
                        std::wcerr << L"Cannot spawn entity of unknown type: "
                                   << packet->entity_type() << std::endl;

                        break;
                }
            }
        }
    }

}
