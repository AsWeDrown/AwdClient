#include "CreateLobbyResponseListener.hpp"
#include "../../graphics/mainmenu/MainMenuScreen.hpp"
#include "../../Game.hpp"

namespace awd::game {

    void CreateLobbyResponseListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        if (Game::instance().getCurrentState() == GameState::LOBBY) {
            auto packet = std::dynamic_pointer_cast<net::CreateLobbyResponse>(packetData->getPacket());
            auto currentScreen = game::Game::instance().getCurrentScreen();

            if (auto mainMenu = std::dynamic_pointer_cast<game::MainMenuScreen>(currentScreen))
                game::MainMenuScreenListener::finishCreateLobby(mainMenu.get(), packet);
        }
    }

}
