#include "BeginQuestListener.hpp"
#include "../../Game.hpp"
#include "../../graphics/play/PlayScreen.hpp"

namespace awd::game {

    void BeginQuestListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        auto packet = std::dynamic_pointer_cast<net::BeginQuest>(packetData->getPacket());
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (Game::instance().getCurrentState() == GameState::PLAY) {
            if (auto playScreen = std::dynamic_pointer_cast<game::PlayScreen>(currentScreen)) {
                std::wcout << L"Begin quest: " << packet->quest_id() << L" / " << packet->quest_type()
                    << L" / " << packet->max_progress() << std::endl;
            }
        }
    }

}
