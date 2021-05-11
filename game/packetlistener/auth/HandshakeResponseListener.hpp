#pragma once


#include "../PacketListener.hpp"

namespace awd::game {

    class HandshakeResponseListener : public PacketListener {
    public:
        void processPacket(
                const std::shared_ptr<google::protobuf::Message>& basePacket) override;
    };

}
