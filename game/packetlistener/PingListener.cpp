#include "PingListener.hpp"
#include "../Game.hpp"

namespace awd::game {

    void PingListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        auto packet = std::dynamic_pointer_cast<net::Ping>(basePacket);
        std::wcout << L"RECEIVED Ping PACKET: testId = " << packet->test_id()
                   << L", latency = " << packet->last_latency() << L" ms" << std::endl;

        Game::instance().getNetService()->pong(packet->test_id());
    }

}
