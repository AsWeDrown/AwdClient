#include "NetworkHandle.hpp"
#include "SequenceNumberMath.hpp"
#include <bitset>

namespace awd::net {

    /* Этот метод НЕ гарантирует потокобезопасность. Его вызов должен быть обёрнут в блокирующий блок. */
    uint32_t NetworkHandle::calculateAckBitfield() {
        uint32_t ackBitfield = 0;

        // Идём от 1, т.к. иначе мы включим в ack bitfield ещё и пакет с номером remote sequence
        // number - а это нам не нужно - о получении пакета с этим номером мы сообщим отправителю
        // напрямую, когда сами будем отправлять ему свой следующий пакет.
        for (uint32_t bitNum = 1; bitNum < PACKETS_QUEUE_SIZE; bitNum++)
            if (std::find(receivedQueue.cbegin(), receivedQueue.cend(),
                          SequenceNumberMath::subtract(remoteSequenceNumber, bitNum))
                          != receivedQueue.cend()) // в порядке уменьшения номера
                // "Оповещение" о том, что мы успешно получили пакет с этим
                // sequence number (устанавливаем бит с соотв. номером на единицу).
                ackBitfield |= 1UL << (bitNum - 1); // т.к. идём с единицы, не забываем отнимать эту единицу здесь

        printf("** TEMP DEBUG ** Calculated ack bitfield: %s\n",
               std::bitset< 32 >( ackBitfield ).to_string().c_str());

        return ackBitfield;
    }

    /* Этот метод гарантирует потокобезопасность. Его вызов должен НЕ быть обёрнут в блокирующий блок. */
    void NetworkHandle::packetReceived(const std::shared_ptr<UnwrappedPacketData>& data) {
        std::unique_lock<std::mutex> lock(mutex);

        // Обновляем удалённый sequence number и добавляем sequence number этого пакета
        // в список недавно полученных (для дальнейшего "оповещения" о получении).
        uint32_t sequence = data->getSequence();

        if (SequenceNumberMath::isMoreRecent(sequence, remoteSequenceNumber))
            // Полученный только что пакет оказался "новее" пакета,
            // sequence number которого у нас сейчас сохранён. Обновляем.
            remoteSequenceNumber = sequence;

        // Смотрим, подтверждение получения каких пакетов другая сторона указала.
        uint32_t ack = data->getAck(); // sequence number последнего пакета, который другая сторона от нас получила
        uint32_t ackBitfield = data->getAckBitfield(); // сведения о получении 32 пакетов до пакета с номером ack
        uint32_t bitNum = 1; // начинаем с 1, т.к. 0 - это этот с номером ack; нас интересуют те, что были до него

        printf("** TEMP DEBUG ** Packet received: #%d (current remote seq: #%d), ack: %d, ack bitfield: %s\n",
               sequence, remoteSequenceNumber, ack, std::bitset< 32 >( ackBitfield ).to_string().c_str());

        while (ackBitfield > 0) {
            bool bitSet = (ackBitfield & 1) == 1;

            if (bitSet) {
                // Другая сторона подтвердила получение от нас пакета с номером (ack - bitNum).
                uint32_t ackedPacketSeqNum = SequenceNumberMath::subtract(ack, bitNum);
                packetDelivered(ackedPacketSeqNum);
            }

            // Переходим к следующему биту.
            ackBitfield >>= 1UL;
            bitNum++;
        }

        // Проверяем подтверждение другой стороной получения от нас отправленных ранее пакетов.
        // Если sequence = ack = 0, то, скорее всего, это первый пакет, который мы получили, и
        // без этой проверки (if) можно сделать ложный вывод о том, что другая сторона подтвердила
        // получение от нас пакета с sequence number #0. При этом в данном случае не страшно "потерять"
        // это подтверждение, т.к. мы всё равно получим его в последующих пакетах в ack bitfield.
        if (sequence != 0 || ack != 0)
            // Другая сторона подтвердила получение от нас пакета с номером ack.
            packetDelivered(ack);

        if (receivedQueue.size() == PACKETS_QUEUE_SIZE)
            // Удаляем самый "старый" пакет из очереди полученных.
            receivedQueue.pop_front();

        // Запоминаем sequence number только что полученного пакета.
        receivedQueue.push_back(sequence);
    }

    /* Этот метод НЕ гарантирует потокобезопасность. Его вызов должен быть обёрнут в блокирующий блок. */
    void NetworkHandle::packetSent(const std::shared_ptr<PacketContainer>& pContainer) {
        // Обновляем локальный sequence number только после успешной отправки этого пакета.
        if (++localSequenceNumber == SEQUENCE_NUMBER_WRAP_AROUND_THRESHOLD)
            localSequenceNumber = 0; // wrap-around

        // Запоминаем этот пакет (но только в случае успешной отправки).
        // В случае потери пакета это поможет отправить его повторно, но
        // уже с другими sequence number, ack и ack bitfield.
        if (sentQueue.size() == PACKETS_QUEUE_SIZE) {
            // Удаляем самый "старый" пакет из очереди отправленных.
            auto oldestPacketSent = sentQueue.front();
            sentQueue.pop_front();

            // Проверяем, не приходило ли нам "оповещение" об успешном получении этого пакета.
            if (std::find(deliveredQueue.cbegin(), deliveredQueue.cend(),
                          oldestPacketSent->getOriginalSequence()) == deliveredQueue.cend())
                // Судя по всему, "оповещения" о получении этого пакета нам
                // пока не приходило. Скорее всего, пакет не дошёл до цели.
                packetPossiblyLost(oldestPacketSent);
        }

        // Запоминаем sequence number только что отправленного пакета.
        sentQueue.push_back(pContainer);
    }

    /* Этот метод НЕ гарантирует потокобезопасность. Его вызов должен быть обёрнут в блокирующий блок. */
    void NetworkHandle::packetDelivered(uint32_t sequence) {
        // Пакет был успешно доставлен. Это совершенно точно.
        // Добавляем его sequence number в очередь доставленных.
        if (deliveredQueue.size() == PACKETS_QUEUE_SIZE)
            deliveredQueue.pop_front(); // удаляем самый "старый" элемент из очереди доставленных

        // Запоминаем sequence number пакета, об успешной
        // доставке которого нам только что стало известно.
        deliveredQueue.push_back(sequence);

        // Учитываем доставку пакета в статистике.
        updateDeliveryStat(true);
        printf("** TEMP DEBUG ** Packet delivered: #%d, "
               "new packet loss: %.2f\n", sequence, packetLossPercent);
    }

    /* Этот метод НЕ гарантирует потокобезопасность. Его вызов должен быть обёрнут в блокирующий блок. */
    void NetworkHandle::packetPossiblyLost(const std::shared_ptr<PacketContainer>& pContainer) {
        // С момента, как мы отправили этот пакет, мы уже успели отправить 32 новых.
        // Однако подтверждения о доставке этого пакета мы так и не получили. Скорее
        // всего, пакет был потерян где-то "по дороге" (хотя и не обязательно - могло
        // случиться и такое, что это просто до нас не дошла информация о его получении).
        if (pContainer->shouldEnsureDelivered())
            // Этот пакет обязательно нужно доставить. Пробуем снова.
            // При этом конструируем новый пакет - лишь оставляя оригинальное содержимое (сообщение).
            sendPacket(true, pContainer->getOriginalPacket());

        // Учитываем потерю пакета в статистике.
        updateDeliveryStat(false);
        printf("** TEMP DEBUG ** Packet possibly lost: #%d, "
               "new packet loss: %.2f\n", pContainer->getOriginalSequence(), packetLossPercent);
    }

    /* Этот метод НЕ гарантирует потокобезопасность. Его вызов должен быть обёрнут в блокирующий блок. */
    void NetworkHandle::updateDeliveryStat(bool deliveredSuccessfully) {
        if (deliveryStats.size() == PACKETS_QUEUE_SIZE)
            deliveryStats.pop_front(); // удаляем запись о самом "старом" отправленном пакете

        deliveryStats.push_back(deliveredSuccessfully ? 1 : 0);

        uint32_t packetsSent = deliveryStats.size();
        uint32_t packetsDelivered = 0;

        for (uint32_t status : deliveryStats)
            packetsDelivered += status;

        packetLossPercent = 100.0f * (packetsSent - packetsDelivered) / packetsSent;
    }

    NetworkHandle::NetworkHandle(const std::shared_ptr<UdpClient>& udpClient) {
        std::wcerr << L"NetworkHandle <init> : udpClient null = " << (udpClient == nullptr) << std::endl;
        this->udpClient = udpClient;
        std::wcerr << L"NetworkHandle <init> : this->udpClient null = " << (this->udpClient == nullptr) << std::endl;
    }

    float NetworkHandle::getPacketLossPercent() const {
        return packetLossPercent;
    }

    std::shared_ptr<UnwrappedPacketData> NetworkHandle::receivePacket(const std::shared_ptr<char[]>& buffer,
                                                                      std::size_t bytesReceived) {
        auto unwrappedPacketData = unwrap(buffer.get(), bytesReceived);

        if (unwrappedPacketData != nullptr)
            // "Протоколообразующие" манипуляции.
            packetReceived(unwrappedPacketData);

        return unwrappedPacketData;
    }

    bool NetworkHandle::sendPacket(bool ensureDelivered,
                                   const std::shared_ptr<google::protobuf::Message>& packet) {
//        if (!udpClient->isConnected()) {
//            std::wcerr << L"Cannot send a packet while not connected." << std::endl;
//            return false; // ошибка ("соединение" не установлено)
//        }

        std::unique_lock<std::mutex> lock(mutex);

        try {
            // Конструируем пакет.
            auto wrappedPacketData = wrap(packet.get(),
                                          localSequenceNumber,
                                          remoteSequenceNumber,
                                          calculateAckBitfield()
            );

            printf("** TEMP DEBUG ** Sending packet #%d, acking #%d\n",
                   localSequenceNumber, remoteSequenceNumber);

            sf::IpAddress serverAddr = udpClient->getServerAddrStr();

            std::wcerr << L"sending packet; udpClient null = " << (udpClient == nullptr ) << std::endl;

            // Отправляем пакет по UDP.
            if (udpClient->getUdpSocket()->send(
                    wrappedPacketData->getData().get(),
                    wrappedPacketData->getDataLen(),
                    serverAddr,
                    udpClient->getPort()) != sf::Socket::Done) {
                std::wcerr << L"Failed to send a packet (UDP error)." << std::endl;
                return false; // ошибка (P.S.: даже в non-blocking режиме при успехе всегда возращается Done)
            }

            // "Протоколообразующие" манипуляции.
            packetSent(std::make_shared<PacketContainer>(
                    ensureDelivered, localSequenceNumber, packet));

            return true; // пакет отправлен успешно
        } catch (const std::exception& ex) {
            std::wcerr << L"Failed to send a packet "
                          L"(internal error: " << ex.what() << L")." << std::endl;
            return false; // ошибка (скорее всего, отсутствует обработчик этого пакета (исп. awd-ptrans-codegen))
        }
    }

}
