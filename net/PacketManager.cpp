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

        if (cursor != listeners.cend())
            cursor->second->processPacket(packet);
        else
            std::wcerr << L"Ignoring unknown packet (no corresponding listener)." << std::endl;
    }

    bool PacketManager::sendPacket(google::protobuf::Message* packet) {
        if (!udpClient->isConnected()) {
            std::wcerr << L"Cannot send a packet while not connected." << std::endl;
            return false; // ошибка
        }

        try {
            auto wrappedPacketData = wrap(packet);
            sf::IpAddress serverAddr = udpClient->getServerAddrStr();

            if (udpClient->getUdpSocket()->send(
                    wrappedPacketData->getData().get(),
                    wrappedPacketData->getDataLen(), serverAddr,
                    udpClient->getPort()) != sf::Socket::Done) {
                std::wcerr << L"Failed to send a packet (UDP error)." << std::endl;
                return false; // ошибка (P.S.: даже в non-blocking режиме при успехе всегда возращается Done)
            }

            return true; // пакет отправлен успешно
        } catch (const std::exception& ex) {
            std::wcerr << L"Failed to send a packet "
                          L"(internal error: " << ex.what() << L")." << std::endl;
            return false; // ошибка (скорее всего, отсутствует обработчик этого пакета (исп. awd-ptrans-codegen))
        }
    }

}
