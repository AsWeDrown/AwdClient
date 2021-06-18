#pragma once


#define PACKETS_QUEUE_SIZE 33 /* = 32+1 - макс. число "недавних" пакетов, данные/сведения о
                              которых мы храним (последний пакет плюс 32 пакета до него)  */


#include <deque>
#include "PacketContainer.hpp"
#include "PacketTransformer.hpp"
#include "UdpClient.hpp"

namespace awd::net {

    class NetworkHandle {
    private:
        std::mutex mutex;

        std::shared_ptr<UdpClient> udpClient = nullptr;

        /**
         * 33 самых "новых" пакета, которые мы отправили другой стороне
         * (хранится в том числе и содержимое этих пакетов для возможности
         * повторной отправки, например, при потере).
         */
        std::deque<std::shared_ptr<PacketContainer>> sentQueue;

        /**
         * Числа sequence number вплоть до 33 пакетов, подтверждение об успешной
         * доставке которых мы "недавно" получили от другой стороны. Используется
         * для возможности делать предположения о потерянных пакетах.
         */
        std::deque<uint32_t> deliveredQueue;

        /**
         * Числа sequence number вплоть до 33 пакетов, которые мы недавно получили.
         * Используется для вычисления и отправке другой стороне ack bitfield.
         */
        std::deque<uint32_t> receivedQueue;

        /**
         * Статусы доставки вплоть до 33 пакетов, которые мы недавно отправили.
         * При получении подтверждения о доставке, в эту очередь добавляется
         * единица. При подозрении на потерю пакета, в неё добавляется ноль.
         * Используется для подсчёта packet loss %.
         */
         std::deque<uint32_t> deliveryStats;

         float packetLossPercent = 0.0f;

        /**
         * "Протоколообразующие" поля.
         */
        std::atomic<uint32_t> localSequenceNumber  = 0, // новер самого "нового" отправленного пакета;
                              remoteSequenceNumber = 0, // номер самого "нового" полученного пакета;
                              newestAck            = 0; // номер самого "нового" отправленного пакета, ...
                                                        // ... получение которого другая сторона подтвердила;

        uint32_t calculateAckBitfield();
        void packetReceived(const std::shared_ptr<UnwrappedPacketData>& data);
        void packetSent(const std::shared_ptr<PacketContainer>& pContainer);
        void packetDelivered(uint32_t sequence);
        void packetPossiblyLost(const std::shared_ptr<PacketContainer>& pContainer);
        void updateDeliveryStat(bool deliveredSuccessfully);

    public:
        explicit NetworkHandle(const std::shared_ptr<UdpClient>& udpClient);

        uint32_t getLocalSequenceNumber ();
        uint32_t getRemoteSequenceNumber();
        uint32_t getNewestAck           ();

        float getPacketLossPercent();

        std::shared_ptr<UnwrappedPacketData> receivePacket(const std::shared_ptr<char[]>& buffer,
                                                           std::size_t bytesReceived);

        /**
         * @return true - пакет отправлен успешно, false - пакет отправить не удалось.
         */
        bool sendPacket(bool ensureDelivered,
                        const std::shared_ptr<google::protobuf::Message>& packet);
    };

}

