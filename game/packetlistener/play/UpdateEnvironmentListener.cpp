#include "UpdateEnvironmentListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/play/PlayScreen.hpp"

namespace awd::game {

    void UpdateEnvironmentListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        auto packet = std::dynamic_pointer_cast<net::UpdateEnvironment>(packetData->getPacket());
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (Game::instance().getCurrentState() == GameState::PLAY)
            if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen))
                playScreen->getEnvironment()->update(packet->env_bitfield());
    }

}
