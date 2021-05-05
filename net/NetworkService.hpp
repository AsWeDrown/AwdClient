#pragma once


#include "PacketManager.hpp"

namespace awd::net {

    class NetworkService {
        std::shared_ptr<PacketManager> packetManager = nullptr;

    public:
        explicit NetworkService(const std::shared_ptr<PacketManager>& packetManager);

        void handshakeRequest();
        void pong();
    };

}

