#include <thread>
#include <algorithm>
#include "PacketManager.hpp"

namespace awd::net {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    PacketManager::PacketManager(const std::shared_ptr<UdpClient>& udpClient) {
        this->handle = std::make_shared<NetworkHandle>(udpClient);
    }

    void PacketManager::registerListener(PacketWrapper::PacketCase packetType,
                                         const std::shared_ptr<game::PacketListener>& listener) {
        listeners[packetType] = listener;
    }

    std::shared_ptr<NetworkHandle> PacketManager::getHandle() const {
        return handle;
    }

    std::shared_ptr<UnwrappedPacketData> PacketManager::receivePacket(const std::shared_ptr<char[]>& buffer,
                                                                      std::size_t bytesReceived) {
        return handle->receivePacket(buffer, bytesReceived);
    }

    void PacketManager::processReceivedPacket(const std::shared_ptr<UnwrappedPacketData>& packetData) {
        // Передаём этот пакет на обработку соответствующему PacketListener'у.
        auto cursor = listeners.find(packetData->getPacketType());

        if (cursor != listeners.cend())
            cursor->second->processPacket(packetData);
        else
            // Protobuf смог десериализовать полученный пакет, но для него в listeners
            // не зарегистрировано (в конструкторе этого класса) подходящих PacketListener'ов.
            std::wcerr << L"Ignoring unknown packet (no corresponding listener)." << std::endl;
    }

    bool PacketManager::sendPacket(const std::shared_ptr<google::protobuf::Message>& packet) {
        return handle->sendPacket(false, packet);
    }

    bool PacketManager::sendImportantPacket(const std::shared_ptr<google::protobuf::Message>& packet) {
        return handle->sendPacket(true, packet);
    }

}
