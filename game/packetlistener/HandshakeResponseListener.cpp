#include "HandshakeResponseListener.hpp"

namespace awd::game {

    void HandshakeResponseListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        auto packet = std::dynamic_pointer_cast<net::HandshakeResponse>(basePacket);
        //TODO crash if mismatch
        std::wcout << L"RECEIVED HandshakeResponse PACKET: protocolVersion=" << packet->protocol_version() << std::endl;
    }

}
