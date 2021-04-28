#include <thread>
#include "CreateLobbyResponseListener.hpp"

namespace awd::game {

    void CreateLobbyResponseListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        auto packet = std::dynamic_pointer_cast<net::CreateLobbyResponse>(basePacket);

        std::cout << "RECEIVED CreateLobbyResponse {" << std::endl;
        std::cout << "  lobbyId = " << packet->lobby_id() << std::endl;
        std::cout << "  playerId = " << packet->player_id() << std::endl;
        std::cout << "}" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

}
