#include <packets.pb.h>

#include <utility>
#include <cxxabi.h>
#include "PacketTransformer.hpp"

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

        if (auto* handshakerequest = dynamic_cast<HandshakeRequest*>(packet))
            wrapper.set_allocated_handshakerequest(handshakerequest);
        else if (auto* handshakeresponse = dynamic_cast<HandshakeResponse*>(packet))
            wrapper.set_allocated_handshakeresponse(handshakeresponse);
        else if (auto* createlobbyrequest = dynamic_cast<CreateLobbyRequest*>(packet))
            wrapper.set_allocated_createlobbyrequest(createlobbyrequest);
        else if (auto* createlobbyresponse = dynamic_cast<CreateLobbyResponse*>(packet))
            wrapper.set_allocated_createlobbyresponse(createlobbyresponse);
        else if (auto* joinlobbyrequest = dynamic_cast<JoinLobbyRequest*>(packet))
            wrapper.set_allocated_joinlobbyrequest(joinlobbyrequest);
        else if (auto* joinlobbyresponse = dynamic_cast<JoinLobbyResponse*>(packet))
            wrapper.set_allocated_joinlobbyresponse(joinlobbyresponse);
        else if (auto* leavelobbyrequest = dynamic_cast<LeaveLobbyRequest*>(packet))
            wrapper.set_allocated_leavelobbyrequest(leavelobbyrequest);
        else if (auto* leavelobbyresponse = dynamic_cast<LeaveLobbyResponse*>(packet))
            wrapper.set_allocated_leavelobbyresponse(leavelobbyresponse);
        else if (auto* kickedfromlobby = dynamic_cast<KickedFromLobby*>(packet))
            wrapper.set_allocated_kickedfromlobby(kickedfromlobby);
        else if (auto* updatedmemberslist = dynamic_cast<UpdatedMembersList*>(packet))
            wrapper.set_allocated_updatedmemberslist(updatedmemberslist);
        else if (auto* keepalive = dynamic_cast<KeepAlive*>(packet))
            wrapper.set_allocated_keepalive(keepalive);
        else if (auto* ping = dynamic_cast<Ping*>(packet))
            wrapper.set_allocated_ping(ping);
        else if (auto* pong = dynamic_cast<Pong*>(packet))
            wrapper.set_allocated_pong(pong);
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
            case PacketWrapper::PacketCase::kHandshakeRequest:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<HandshakeRequest>(wrapper.handshakerequest()));

            case PacketWrapper::PacketCase::kHandshakeResponse:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<HandshakeResponse>(wrapper.handshakeresponse()));

            case PacketWrapper::PacketCase::kCreateLobbyRequest:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<CreateLobbyRequest>(wrapper.createlobbyrequest()));

            case PacketWrapper::PacketCase::kCreateLobbyResponse:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<CreateLobbyResponse>(wrapper.createlobbyresponse()));

            case PacketWrapper::PacketCase::kJoinLobbyRequest:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<JoinLobbyRequest>(wrapper.joinlobbyrequest()));

            case PacketWrapper::PacketCase::kJoinLobbyResponse:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<JoinLobbyResponse>(wrapper.joinlobbyresponse()));

            case PacketWrapper::PacketCase::kLeaveLobbyRequest:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<LeaveLobbyRequest>(wrapper.leavelobbyrequest()));

            case PacketWrapper::PacketCase::kLeaveLobbyResponse:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<LeaveLobbyResponse>(wrapper.leavelobbyresponse()));

            case PacketWrapper::PacketCase::kKickedFromLobby:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<KickedFromLobby>(wrapper.kickedfromlobby()));

            case PacketWrapper::PacketCase::kUpdatedMembersList:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<UpdatedMembersList>(wrapper.updatedmemberslist()));

            case PacketWrapper::PacketCase::kKeepAlive:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<KeepAlive>(wrapper.keepalive()));

            case PacketWrapper::PacketCase::kPing:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<Ping>(wrapper.ping()));

            case PacketWrapper::PacketCase::kPong:
                return std::make_shared<UnwrappedPacketData>(packetType,
                        std::make_shared<Pong>(wrapper.pong()));

            default:
                // Неизвестный пакет - он будет проигнорирован (не передан никакому PacketListener'у).
                return nullptr;
        }
    }

}
