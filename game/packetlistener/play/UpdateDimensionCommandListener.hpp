#pragma once


#include "../PacketListener.hpp"

namespace awd::game {

    class UpdateDimensionCommandListener : public PacketListener {
        void processPacket(
                const std::shared_ptr<net::UnwrappedPacketData>& packetData) override;
    };

}


