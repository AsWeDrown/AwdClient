#include <thread>
#include "PacketManager.hpp"
#include "PacketTransformer.hpp"

namespace awd::net {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    PacketManager::PacketManager(std::shared_ptr<UdpClient> udpClient) {
        this->udpClient = std::move(udpClient);
    }

    void PacketManager::registerListener(PacketWrapper::PacketCase packetType,
                                         const std::shared_ptr<game::PacketListener>& listener) {
        listeners[packetType] = listener;
    }

    void PacketManager::receivePacket(PacketWrapper::PacketCase packetType,
                                      const std::shared_ptr<google::protobuf::Message>& packet) {
        auto cursor = listeners.find(packetType);

        if (cursor != listeners.end())
            cursor->second->processPacket(packet);
        else
            std::cerr << "Ignoring unknown packet (no corresponding listener)." << std::endl;
    }

    bool PacketManager::sendPacket(google::protobuf::Message* packet) {
        if (!udpClient->isConnected()) {
            std::cerr << "Cannot send a packet while not connected" << std::endl;
            return false; // ошибка
        }

        auto wrappedPacketData = wrap(packet);
        sf::IpAddress serverAddr = udpClient->getServerAddrStr();

        if (udpClient->getUdpSocket()->send(
                wrappedPacketData->getData().get(),
                wrappedPacketData->getDataLen(), serverAddr,
                udpClient->getPort()) != sf::Socket::Done) {
            std::cerr << "Failed to send a packet." << std::endl;
            return false; // ошибка (P.S.: даже в non-blocking режиме при успехе всегда возращается Done)
        }

        return true; // пакет отправлен успешно
    }

}
