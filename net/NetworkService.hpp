#pragma once


#include "PacketManager.hpp"

namespace awd::net {

    class NetworkService {
    private:
        std::recursive_mutex mutex;

        std::shared_ptr<PacketManager> packetManager = nullptr;

        std::deque<std::shared_ptr<UnwrappedPacketData>> receiveQueue;
        std::deque<std::shared_ptr<google::protobuf::Message>> sendQueue;
        std::deque<bool> ensureDeliveredStatuses;

        void enqueueSend(bool ensureDelivered,
                         const std::shared_ptr<google::protobuf::Message>& packet);

    public:
        explicit NetworkService(const std::shared_ptr<PacketManager>& packetManager);

        ////////////////////////////////////////////////////////////////////////////////////////////
        //   Управление очередями пакетов.
        ////////////////////////////////////////////////////////////////////////////////////////////

        void enqueueReceive(const std::shared_ptr<UnwrappedPacketData>& packet);

        void enqueueSend(const std::shared_ptr<google::protobuf::Message>& packet);

        void enqueueSendImportant(const std::shared_ptr<google::protobuf::Message>& packet);

        void flushReceiveQueue();

        void flushSendQueue();

        ////////////////////////////////////////////////////////////////////////////////////////////
        //   Работа с пакетами.
        ////////////////////////////////////////////////////////////////////////////////////////////

        void handshakeRequest();

        void pong(uint32_t testId);
    };

}

