#include "NetworkService.hpp"
#include "../game/util/TimeUtils.hpp"

namespace awd::net {

    NetworkService::NetworkService(const std::shared_ptr<PacketManager>& packetManager) {
        this->packetManager = packetManager;
    }

    void NetworkService::handshakeRequest() {
        auto packet = std::make_shared<HandshakeRequest>();
        packet->set_protocol_version(PacketManager::PROTOCOL_VERSION);

        packetManager->sendImportantPacket(packet);
    }

    void NetworkService::pong() {
        auto packet = std::make_shared<Pong>();
        packet->set_client_time(game::TimeUtils::currentTimeMillis());

        packetManager->sendPacket(packet);
    }

}
