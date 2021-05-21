#pragma once


#include "../PacketListener.hpp"

namespace awd::game {

    class LeaveLobbyResponseListener : public PacketListener {
        void processPacket(
                const std::shared_ptr<net::UnwrappedPacketData>& packetData) override;
    };

}

