#include "PingListener.hpp"
#include "../../Game.hpp"

namespace awd::game {

    void PingListener::processPacket(
            const std::shared_ptr<net::UnwrappedPacketData>& packetData) {
        auto packet = std::dynamic_pointer_cast<net::Ping>(packetData->getPacket());
        Game::instance().setLastRtt(packet->rtt());
        Game::instance().getNetService()->pong(packet->test_id());
    }

}
