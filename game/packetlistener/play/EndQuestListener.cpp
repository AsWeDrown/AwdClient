#include "EndQuestListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/play/PlayScreen.hpp"

namespace awd::game {

    void EndQuestListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        auto packet = std::dynamic_pointer_cast<net::EndQuest>(packetData->getPacket());
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (Game::instance().getCurrentState() == GameState::PLAY) {
            if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen)) {
                std::wcout << L"End quest: " << packet->quest_id() << L" / " << packet->status() << std::endl;
            }
        }
    }

}
