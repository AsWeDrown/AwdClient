#include <thread>
#include "CreateLobbyResponseListener.hpp"

namespace awd::game {

    void CreateLobbyResponseListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        auto packet = std::dynamic_pointer_cast<net::CreateLobbyResponse>(basePacket);

        std::wcout << L"RECEIVED CreateLobbyResponse {" << std::endl;
        std::wcout << L"  lobbyId = " << packet->lobby_id() << std::endl;
        std::wcout << L"  playerId = " << packet->player_id() << std::endl;
        std::wcout << L"}" << std::endl;
    }

}
