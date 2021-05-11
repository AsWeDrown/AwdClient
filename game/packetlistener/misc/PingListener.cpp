#include "PingListener.hpp"
#include "../../Game.hpp"

namespace awd::game {

    void PingListener::processPacket(
            const std::shared_ptr<google::protobuf::Message>& basePacket) {
        auto packet = std::dynamic_pointer_cast<net::Ping>(basePacket);
        Game::instance().setLastRtt(packet->rtt());
        Game::instance().getNetService()->pong(packet->test_id());
    }

}
