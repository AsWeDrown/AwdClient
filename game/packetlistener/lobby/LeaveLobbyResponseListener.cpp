#include "LeaveLobbyResponseListener.hpp"
#include "../../graphics/lobby/LobbyScreen.hpp"
#include "../../Game.hpp"

namespace awd::game {

    void LeaveLobbyResponseListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        if (Game::instance().getCurrentState() == GameState::LOBBY) {
            auto packet = std::dynamic_pointer_cast<net::LeaveLobbyResponse>(packetData->getPacket());
            auto currentScreen = game::Game::instance().getCurrentScreen();

            if (auto lobbyScreen = std::dynamic_pointer_cast<game::LobbyScreen>(currentScreen))
                game::LobbyScreenListener::finishLeaveLobby(lobbyScreen.get(), packet);
        }
    }

}
