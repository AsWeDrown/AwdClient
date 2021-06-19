#include "DespawnEntityListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/play/PlayScreen.hpp"
#include "../../entity/EntityPlayer.hpp"

namespace awd::game {

    void DespawnEntityListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        auto packet = std::dynamic_pointer_cast<net::DespawnEntity>(packetData->getPacket());
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (Game::instance().getCurrentState() == GameState::PLAY) {
            if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen)) {
                std::wcout << L"Despawned entity " << packet->entity_id() << std::endl;
                auto despawnedEntity = playScreen->getWorld()->getEntityById(packet->entity_id());

                if (auto somePlayer = std::dynamic_pointer_cast<game::EntityPlayer>(despawnedEntity)) {
                    // Очередной костыль. Этим* должен заниматься сервер.
                    // [ * завершением игры в комнате при выходе любого её участника ]
                    std::wcout << L"A player has left. Leaving the lobby as well! [Game end implied.]" << std::endl;
                    std::wcout << L"Left player ID: " << somePlayer->playerId << L"." << std::endl;
                    Game::instance().returnToMainMenu();
                } else
                    playScreen->getWorld()->removeEntity(packet->entity_id());
            }
        }
    }

}
