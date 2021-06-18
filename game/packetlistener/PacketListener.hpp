#pragma once


#include <memory>
#include <google/protobuf/message.h>
#include "packets.pb.h"
#include "../net/PacketTransformer.hpp"

namespace awd::game {

    class PacketListener {
    public:
        virtual void processPacket(
                const std::shared_ptr<net::UnwrappedPacketData>& packetData) = 0;
    };

}
