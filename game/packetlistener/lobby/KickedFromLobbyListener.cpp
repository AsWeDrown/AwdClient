#include "KickedFromLobbyListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/lobby/LobbyScreen.hpp"

namespace awd::game {

    void KickedFromLobbyListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        if (Game::instance().getCurrentState() == GameState::LOBBY) {
            auto packet = std::dynamic_pointer_cast<net::KickedFromLobby>(packetData->getPacket());
            auto currentScreen = game::Game::instance().getCurrentScreen();

            if (auto lobbyScreen = std::dynamic_pointer_cast<game::LobbyScreen>(currentScreen))
                game::LobbyScreenListener::kickedFromLobby(lobbyScreen.get(), packet);
        }
    }

}
