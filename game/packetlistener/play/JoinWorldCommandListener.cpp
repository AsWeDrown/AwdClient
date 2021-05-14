#include "JoinWorldCommandListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/lobby/LobbyScreen.hpp"

namespace awd::game {

    void JoinWorldCommandListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        if (Game::instance().getCurrentState() == GameState::PLAY) {
            auto packet = std::dynamic_pointer_cast<net::JoinWorldCommand>(basePacket);
            auto currentScreen = game::Game::instance().getCurrentScreen();

            if (auto lobbyScreen = std::dynamic_pointer_cast<game::LobbyScreen>(currentScreen))
                // Переход к игровому экрану.
                game::LobbyScreenListener::joinWorld(lobbyScreen.get(), packet);
        }
    }

}
