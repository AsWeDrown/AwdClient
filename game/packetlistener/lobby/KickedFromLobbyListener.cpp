#include "KickedFromLobbyListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/lobby/LobbyScreen.hpp"

namespace awd::game {

    void KickedFromLobbyListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        auto packet = std::dynamic_pointer_cast<net::KickedFromLobby>(basePacket);
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (auto lobbyScreen = std::dynamic_pointer_cast<game::LobbyScreen>(currentScreen))
            game::LobbyScreenListener::kickedFromLobby(lobbyScreen.get(), packet);
    }

}
