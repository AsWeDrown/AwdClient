#include "DisplayChatMessageListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/play/PlayScreen.hpp"
#include "../../util/StringUtils.hpp"

namespace awd::game {

    void DisplayChatMessageListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        auto packet = std::dynamic_pointer_cast<net::DisplayChatMessage>(packetData->getPacket());
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (Game::instance().getCurrentState() == GameState::PLAY)
            if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen))
                playScreen->getChat()->addChatMessage(
                        StringUtils::getUtf8WideString(packet->message()));
    }

}
