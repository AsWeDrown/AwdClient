#include "AdvanceQuestListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/play/PlayScreen.hpp"

namespace awd::game {

    void AdvanceQuestListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        auto packet = std::dynamic_pointer_cast<net::AdvanceQuest>(packetData->getPacket());
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (Game::instance().getCurrentState() == GameState::PLAY)
            if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen))
                playScreen->getQuestManager()->advanceQuest(packet->quest_id(), packet->progress());
    }

}
