#include "PingListener.hpp"
#include "../util/TimeUtils.hpp"

namespace awd::game {

    void PingListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        auto packet = std::dynamic_pointer_cast<net::Ping>(basePacket);
        std::wcout << L"RECEIVED Ping PACKET: serverTime=" << packet->server_time() << std::endl;
        std::wcout << L"... latency = " << (game::TimeUtils::currentTimeMillis() - packet->server_time())
                   << L" ms" << std::endl;
    }

}
