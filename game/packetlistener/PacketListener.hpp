#pragma once


#include <memory>
#include <google/protobuf/message.h>
#include "packets.pb.h"

namespace awd::game {

    class PacketListener {
    public:
        virtual void processPacket(
                const std::shared_ptr<google::protobuf::Message>& basePacket) = 0;
    };

}
