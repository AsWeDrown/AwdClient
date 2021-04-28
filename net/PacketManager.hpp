#pragma once


#include <google/protobuf/message.h>
#include <packets.pb.h>
#include <any>
#include "UdpClient.hpp"
#include "../game/packetlistener/PacketListener.hpp"

namespace awd::net {

    class PacketManager {
    private:
        std::shared_ptr<UdpClient> udpClient;
        std::map<PacketWrapper::PacketCase, std::shared_ptr<game::PacketListener>> listeners;

    public:
        // TODO - МЕНЯТЬ ЗДЕСЬ:
        static const int PROTOCOL_VERSION = 1;

        explicit PacketManager(std::shared_ptr<UdpClient> udpClient);

        void registerListener(PacketWrapper::PacketCase packetType,
                              const std::shared_ptr<game::PacketListener>& listener);

        void receivePacket(PacketWrapper::PacketCase packetType,
                           const std::shared_ptr<google::protobuf::Message>& packet);

        /**
         * @return true - пакет отправлен успешно, false - пакет отправить не удалось.
         */
        bool sendPacket(google::protobuf::Message* packet);
    };

}
