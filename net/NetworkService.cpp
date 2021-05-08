#define MAX_SEND_QUEUE_SIZE 33 /* = размер очередей в NetworkHandle */


#include "NetworkService.hpp"
#include "../game/util/TimeUtils.hpp"

namespace awd::net {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void NetworkService::enqueueSend(bool ensureDelivered,
                                     const std::shared_ptr<google::protobuf::Message>& packet) {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        if (sendQueue.size() == MAX_SEND_QUEUE_SIZE)
            sendQueue.pop_front(); // отменяем отправку самого "старого" пакета (их накопилось слишком много)

        sendQueue.push_back(packet);
        ensureDeliveredStatuses.push_back(ensureDelivered);
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    ////////////////////////////////////////////////////////////////////////////////////////////
    //   Управление очередями пакетов.
    ////////////////////////////////////////////////////////////////////////////////////////////

    NetworkService::NetworkService(const std::shared_ptr<PacketManager>& packetManager) {
        this->packetManager = packetManager;
    }

    void NetworkService::enqueueReceive(const std::shared_ptr<UnwrappedPacketData>& packet) {
        std::unique_lock<std::recursive_mutex> lock(mutex);
        receiveQueue.push_back(packet);
    }

    void NetworkService::enqueueSend(const std::shared_ptr<google::protobuf::Message>& packet) {
        enqueueSend(false, packet);
    }

    void NetworkService::enqueueSendImportant(const std::shared_ptr<google::protobuf::Message>& packet) {
        enqueueSend(true, packet);
    }

    void NetworkService::flushReceiveQueue() {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        if (!receiveQueue.empty()) {
            for (const auto& packet : receiveQueue)
                packetManager->processReceivedPacket(packet);

            receiveQueue.clear();
        }
    }

    void NetworkService::flushSendQueue() {
        std::unique_lock<std::recursive_mutex> lock(mutex);

        if (!sendQueue.empty()) {
            for (int i = 0; i < sendQueue.size(); i++) {
                auto packet = sendQueue[i];
                bool ensureDelivered = ensureDeliveredStatuses[i];

                if (ensureDelivered)
                    packetManager->sendImportantPacket(packet);
                else
                    packetManager->sendPacket(packet);
            }

            sendQueue.clear();
            ensureDeliveredStatuses.clear();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    //   Работа с пакетами.
    ////////////////////////////////////////////////////////////////////////////////////////////

    void NetworkService::handshakeRequest() {
        auto packet = std::make_shared<HandshakeRequest>();
        packet->set_protocol_version(PacketManager::PROTOCOL_VERSION);

        enqueueSendImportant(packet);
    }

    void NetworkService::pong(uint32_t testId) {
        auto packet = std::make_shared<Pong>();
        packet->set_test_id(testId);

        enqueueSend(packet);
    }

}
