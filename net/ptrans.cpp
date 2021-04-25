#include <packets.pb.h>

#include <utility>
#include <cxxabi.h>
#include "ptrans.hpp"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   TODO: при добавлении в протокол новых пакетов ОБЯЗАТЕЛЬНО ДОБАВЛЯТЬ ИХ СЮДА!
 *         Для добавления использовать утилиту-генератор кода awd-ptrans-codegen.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
namespace awd::net {

    /*

     WrappedPacketData

     */

    WrappedPacketData::WrappedPacketData(std::shared_ptr<char[]> data, size_t dataLen) {
        this->data = std::move(data);
        this->dataLen = dataLen;
    }

    std::shared_ptr<char[]> WrappedPacketData::getData() const {
        return data;
    }

    size_t WrappedPacketData::getDataLen() const {
        return dataLen;
    }

    /*

     UnwrappedPacketData

     */

    UnwrappedPacketData::UnwrappedPacketData(PacketWrapper::PacketCase packetType,
                                             std::shared_ptr<google::protobuf::Message> packet) {
        this->packetType = packetType;
        this->packet = std::move(packet);
    }

    PacketWrapper::PacketCase UnwrappedPacketData::getPacketType() const {
        return packetType;
    }

    std::shared_ptr<google::protobuf::Message> UnwrappedPacketData::getPacket() const {
        return packet;
    }

    /*

     Утилити-функции.

     */

    std::shared_ptr<WrappedPacketData> wrap(google::protobuf::Message* packet) {
        if (packet == nullptr)
            throw std::invalid_argument("packet cannot be null");

        return internalGeneratedWrap(packet);
    }

    std::shared_ptr<UnwrappedPacketData> unwrap(char* rawProto3PacketData, size_t rawProto3PacketDataLen) {
        if (rawProto3PacketData == nullptr || rawProto3PacketDataLen == 0)
            throw std::invalid_argument("rawProto3PacketData cannot be null or empty");

        return internalGeneratedUnwrap(rawProto3PacketData, rawProto3PacketDataLen);
    }

    /*

    Код ниже сгенерирован автоматически с помощью утилиты awd-ptrans-codegen.
    Руками не трогать. Не кормить.

     */

    // Сгенерировано с помощью awd-ptrans-codegen.
    std::shared_ptr<WrappedPacketData> internalGeneratedWrap(google::protobuf::Message* packet) {
        PacketWrapper wrapper;

        if (auto* createlobbyrequest = dynamic_cast<CreateLobbyRequest*>(packet))
            wrapper.set_allocated_createlobbyrequest(createlobbyrequest);
        else if (auto* keepalive = dynamic_cast<KeepAlive*>(packet))
            wrapper.set_allocated_keepalive(keepalive);
        else
            // Код "if ..." для пакетов этого типа отсутствует выше.
            // Нужно добавить! (исп. awd-ptrans-codegen)
            throw std::invalid_argument("no implemented transformer for this packet type");

        size_t dataLen = wrapper.ByteSizeLong();
        std::shared_ptr<char[]> data(new char[dataLen]);
        wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));

        return std::make_shared<WrappedPacketData>(data, dataLen);
    }

    // Сгенерировано с помощью awd-ptrans-codegen.
    std::shared_ptr<UnwrappedPacketData> internalGeneratedUnwrap(char* data, size_t dataLen) {
        PacketWrapper wrapper;
        wrapper.ParseFromArray(data, static_cast<int>(dataLen));
        PacketWrapper::PacketCase packetType = wrapper.packet_case();

        switch (packetType) {
            case PacketWrapper::PacketCase::kCreateLobbyRequest:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<CreateLobbyRequest>(wrapper.createlobbyrequest()));

            case PacketWrapper::PacketCase::kKeepAlive:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<KeepAlive>(wrapper.keepalive()));

            default:
                // Неизвестный пакет - он будет проигнорирован (не передан никакому PacketListener'у).
                return nullptr;
        }
    }

}
