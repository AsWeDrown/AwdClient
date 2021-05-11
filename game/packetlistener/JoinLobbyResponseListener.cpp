#include "JoinLobbyResponseListener.hpp"
#include "../game/graphics/mainmenu/MainMenuScreen.hpp"
#include "../Game.hpp"

namespace awd::game {

    void JoinLobbyResponseListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        auto packet = std::dynamic_pointer_cast<net::JoinLobbyResponse>(basePacket);
        auto currentScreen = game::Game::instance().getCurrentScreen();

        if (auto mainMenu = std::dynamic_pointer_cast<game::MainMenuScreen>(currentScreen))
            game::MainMenuScreenListener::finishJoinLobby(mainMenu.get(), packet);
    }

}