#pragma once


#include <deque>
#include <google/protobuf/message.h>
#include <packets.pb.h>
#include <any>
#include "UdpClient.hpp"
#include "../game/packetlistener/PacketListener.hpp"
#include "PacketTransformer.hpp"
#include "PacketContainer.hpp"
#include "NetworkHandle.hpp"

namespace awd::net {

    class PacketManager {
    private:
        std::shared_ptr<NetworkHandle> handle;
        std::map<PacketWrapper::PacketCase, std::shared_ptr<game::PacketListener>> listeners;

    public:
        // TODO - МЕНЯТЬ ЗДЕСЬ:
        static const uint32_t PROTOCOL_VERSION = 1;

        explicit PacketManager(const std::shared_ptr<UdpClient>& udpClient);

        void registerListener(PacketWrapper::PacketCase packetType,
                              const std::shared_ptr<game::PacketListener>& listener);

        std::shared_ptr<NetworkHandle> getHandle() const;

        std::shared_ptr<UnwrappedPacketData> receivePacket(const std::shared_ptr<char[]>& buffer,
                                                           std::size_t bytesReceived);

        void processReceivedPacket(const std::shared_ptr<UnwrappedPacketData>& packetData);
        bool sendPacket(const std::shared_ptr<google::protobuf::Message>& packet);
        bool sendImportantPacket(const std::shared_ptr<google::protobuf::Message>& packet);
    };

}
