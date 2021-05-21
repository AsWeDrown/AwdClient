#include <thread>
#include <algorithm>
#include "PacketManager.hpp"

namespace awd::net {

    /*
     * @@@ BEGIN
     *
     * client sent #1 [1]
     * client sent #2 [1,2]
     * client sent #3 [1,2,3]
     * client sent #4 [1,2,3,4]
     *
     * server received #1
     * server acked #1, bitfield 00000
     *
     * client received ack for #1[nice they got #1] and bitfield 00000[ignored]
     *
     * server received #2
     * server acked #2, bitfield 00001[confirm also received #(2 - 1 = 1))]
     *
     * client received ack for #2[nice they got #2] and bitfield 00001
     *                                                  [nice they also got #(2 - 1 = 1) which I already knew tho]
     *
     * server received #3
     * server acked #3, bitfield 00011[confirm also received #(3 - 1 = 2) and #(3 - 2 = 1)] --- BUT THIS ACK PACKET WAS LOST
     *
     * --- CLIENT DOES NOT KNOW WHETHER SERVER RECEIVED #3 SUCCESSFULLY OR NOT
     *
     * server received #4
     * server acked #4, bitfield 00111[confirm also received #(4 - 1 = 3) and #(4 - 2 = 2) and #(4 - 3 = 1)]
     *
     * client received ack for #4[nice they got #4] and bitfield 00111
     * [nice they also got #(4 - 1 = 3) WHICH I DID NOT KNOW WOW OK GREAT,
     *  nice they also got #(4 - 2 = 2) which I already knew tho,
     *  nice they also got #(4 - 3 = 1) which I already knew tho].
     *
     * client sent #5 --- BUT THIS PACKET WAS LOST
     *
     * --- SERVER DID NOT RECEIVE PACKET #5
     *
     * --- AFTER SOME TIME CLIENT SUPPOSES THAT PACKET #5 THAT IT RECENTLY SENT WAS LOST
     *
     * client sent #6[a copy of #5 but with new seq num]
     *
     * < server received, responded, client knows, etc. >
     *
     * @@@ END --> client KNOWS FOR SURE that ALL packets #1 AND #2 AND #3 AND #4
     *             were SUCCESSFULLY RECEIVED by server; client also thinks that
     *             packet #5 MIGHT HAD BEEN LOST, however, its copy, packet #6,
     *             WAS SUCCESSFULLY RECEIVED BY THE SERVER. All good. End.
     */

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
