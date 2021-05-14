#define MAX_SEND_QUEUE_SIZE 10


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

        if (sendQueue.size() == MAX_SEND_QUEUE_SIZE) {
            // Отменяем отправку самого "старого" пакета (их накопилось уж слишком много).
            sendQueue.pop_front();
            ensureDeliveredStatuses.pop_front();
        }

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
        packetManager->sendPacket(packet); // Для пакетов Ping/Pong используем мгновенные отправку/получение.
    }

    void NetworkService::createLobbyRequest(const std::wstring& playerName) {
        auto packet = std::make_shared<CreateLobbyRequest>();
        packet->set_player_name(std::string(playerName.begin(), playerName.end())); // wstring --> string
        enqueueSendImportant(packet);
    }

    void NetworkService::joinLobbyRequest(uint32_t lobbyId, const std::wstring& playerName) {
        auto packet = std::make_shared<JoinLobbyRequest>();

        packet->set_lobby_id(lobbyId);
        packet->set_player_name(std::string(playerName.begin(), playerName.end())); // wstring --> string

        enqueueSendImportant(packet);
    }

    void NetworkService::leaveLobbyRequest() {
        auto packet = std::make_shared<LeaveLobbyRequest>();
        enqueueSendImportant(packet);
    }

    void NetworkService::beginPlayStateRequest(const std::string& saveId) {
        auto packet = std::make_shared<BeginPlayStateRequest>();
        packet->set_save_id(saveId);
        enqueueSendImportant(packet);
    }

    void NetworkService::updateDimensionComplete() {
        auto packet = std::make_shared<UpdateDimensionComplete>();
        enqueueSendImportant(packet);
    }

    void NetworkService::joinWorldComplete() {
        auto packet = std::make_shared<JoinWorldComplete>();
        enqueueSendImportant(packet);
    }

}
