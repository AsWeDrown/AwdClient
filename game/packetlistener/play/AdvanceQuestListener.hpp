#pragma once


#include "../PacketListener.hpp"

namespace awd::game {

    class AdvanceQuestListener : public PacketListener {
    public:
        void processPacket(
                const std::shared_ptr<net::UnwrappedPacketData>& packetData) override;
    };

}
