#include "HandshakeResponseListener.hpp"
#include "../../Game.hpp"

namespace awd::game {

    void HandshakeResponseListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        if (Game::instance().getCurrentState() == GameState::AUTH) {
            auto packet = std::dynamic_pointer_cast<net::HandshakeResponse>(basePacket);
            uint32_t serverProtocolVersion = packet->protocol_version();
            std::wcout << L"Server protocol version: " << serverProtocolVersion << std::endl;
            Game::instance().handshakeComplete(serverProtocolVersion);
        }
    }

}
