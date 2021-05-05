#include "PacketContainer.hpp"

namespace awd::net {

    PacketContainer::PacketContainer(bool ensureDelivered,
                                     uint32_t sequence,
                                     const std::shared_ptr<google::protobuf::Message>& packet) {
        this->ensureDelivered = ensureDelivered;
        this->sequence = sequence;
        this->packet = packet;
    }

    bool PacketContainer::shouldEnsureDelivered() const {
        return ensureDelivered;
    }

    uint32_t PacketContainer::getOriginalSequence() const {
        return sequence;
    }

    std::shared_ptr<google::protobuf::Message> PacketContainer::getOriginalPacket() const {
        return packet;
    }

}
