#pragma once


#include <google/protobuf/message.h>
#include "packets.pb.h"

namespace awd::net {

    class WrappedPacketData {
    private:
        std::shared_ptr<char[]> data;
        size_t dataLen;

    public:
        WrappedPacketData(const std::shared_ptr<char[]>& data, size_t dataLen);

        std::shared_ptr<char[]> getData() const;
        size_t getDataLen() const;
    };

    class UnwrappedPacketData {
    private:
        uint32_t sequence, ack, ackBitfield;
        PacketWrapper::PacketCase packetType;
        std::shared_ptr<google::protobuf::Message> packet;

    public:
        UnwrappedPacketData(uint32_t sequence,
                            uint32_t ack,
                            uint32_t ackBitfield,
                            PacketWrapper::PacketCase packetType,
                            const std::shared_ptr<google::protobuf::Message>& packet);

        uint32_t getSequence() const;
        uint32_t getAck() const;
        uint32_t getAckBitfield() const;
        PacketWrapper::PacketCase getPacketType() const;
        std::shared_ptr<google::protobuf::Message> getPacket() const;
    };

    /**
     * Принимает на вход сам пакет (то, что мы и должны отправить), преобразовывает (обычный cast)
     * к пакету соответствующего типа (напр., Message --> KeepAlive [implements Message]), создаёт
     * обёртку PacketWrapper над преобразованным пакетов (т.е., напр., не над Message, а прямо над
     * KeepAlive), помещая преобразованный пакет в нужное поле в обёртке (в данном примере это через
     * PacketWrapper#setKeepAlive), а затем сериализует полученную обёртку над этим пакетом в "сырой"
     * массив байтов, который уже можно передавать по сети.
     *
     * @see #unwrap(byte[]) для обратного действия.
     */
    std::shared_ptr<WrappedPacketData> wrap(google::protobuf::Message* packet,
                                            uint32_t sequence,
                                            uint32_t ack,
                                            uint32_t ackBitfield);

    /**
     * Принимает на вход "сырой" массив байтов, полученный от какого-то клиента по UDP,
     * десериализует этот массив байтов в общую обёртку для всех пакетов, PacketWrapper,
     * определяет тип (в данном случае - enum PacketWrapper.PacketCase) пакета, обёрнутого
     * этим PacketWrapper'ом, и распаковывает сам пакет (то, что мы и должны обработать).
     *
     * @see #wrap(Message) для обратного действия.
     */
    std::shared_ptr<UnwrappedPacketData> unwrap(char* rawProto3PacketData,
                                                size_t rawProto3PacketDataLen);

    /*

    Код ниже сгенерирован автоматически с помощью утилиты awd-ptrans-codegen.
    Руками не трогать. Не кормить.

     */

    // Сгенерировано с помощью awd-ptrans-codegen. (ОБЯЗАТЕЛЬНО ДОЛЖНО БЫТЬ В ОДНУ СТРОЧКУ - НИЧЕГО НЕ ПЕРЕНОСИТЬ!!!)
    std::shared_ptr<WrappedPacketData> internalGeneratedWrap(google::protobuf::Message* packet, uint32_t sequence, uint32_t ack, uint32_t ackBitfield);

    // Сгенерировано с помощью awd-ptrans-codegen. (ОБЯЗАТЕЛЬНО ДОЛЖНО БЫТЬ В ОДНУ СТРОЧКУ - НИЧЕГО НЕ ПЕРЕНОСИТЬ!!!)
    std::shared_ptr<UnwrappedPacketData> internalGeneratedUnwrap(char* data, size_t dataLen);

}
