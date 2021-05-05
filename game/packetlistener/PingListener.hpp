#pragma once


#include "PacketListener.hpp"

namespace awd::game {

    class PingListener : public PacketListener {
        void processPacket(
                const std::shared_ptr<google::protobuf::Message>& basePacket) override;
    };

}

