#include "UpdateDimensionCommandListener.hpp"
#include "../../graphics/lobby/LobbyScreen.hpp"
#include "../../Game.hpp"

namespace awd::game {

    void UpdateDimensionCommandListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        auto packet = std::dynamic_pointer_cast<net::UpdateDimensionCommand>(packetData->getPacket());
        auto currentScreen = game::Game::instance().getCurrentScreen();
        auto currentState = Game::instance().getCurrentState();

        if (currentState == GameState::LOBBY) {
            if (auto lobbyScreen = std::dynamic_pointer_cast<game::LobbyScreen>(currentScreen))
                // Первичная загрузка мира (измерения).
                game::LobbyScreenListener::initialUpdateDimension(lobbyScreen.get(), packet);
        } else if (currentState == GameState::PLAY) {
            if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen))
                // Изменение мира (измерения) во время игры.
                playScreen->getWorld()->updateDimension(packet->dimension());
        }
    }

}
