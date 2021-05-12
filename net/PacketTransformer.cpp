#include <packets.pb.h>

#include <utility>
#include <cxxabi.h>
#include "PacketTransformer.hpp"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   TODO: при добавлении в протокол новых пакетов ОБЯЗАТЕЛЬНО ДОБАВЛЯТЬ ИХ СЮДА!
 *         Для добавления использовать утилиту-генератор кода awd-ptrans-codegen.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
namespace awd::net {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    /*

     WrappedPacketData

     */

    WrappedPacketData::WrappedPacketData(const std::shared_ptr<char[]>& data, size_t dataLen) {
        this->data = data;
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

    UnwrappedPacketData::UnwrappedPacketData(uint32_t sequence,
                                             uint32_t ack,
                                             uint32_t ackBitfield,
                                             PacketWrapper::PacketCase packetType,
                                             const std::shared_ptr<google::protobuf::Message>& packet) {
        this->sequence = sequence;
        this->ack = ack;
        this->ackBitfield = ackBitfield;
        this->packetType = packetType;
        this->packet = packet;
    }

    uint32_t UnwrappedPacketData::getSequence() const {
        return sequence;
    }

    uint32_t UnwrappedPacketData::getAck() const {
        return ack;
    }

    uint32_t UnwrappedPacketData::getAckBitfield() const {
        return ackBitfield;
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

    std::shared_ptr<WrappedPacketData> wrap(google::protobuf::Message* packet,
                                            uint32_t sequence,
                                            uint32_t ack,
                                            uint32_t ackBitfield) {
        if (packet == nullptr)
            throw std::invalid_argument("packet cannot be null");

        return internalGeneratedWrap(packet, sequence, ack, ackBitfield);
    }

    std::shared_ptr<UnwrappedPacketData> unwrap(char* rawProto3PacketData,
                                                size_t rawProto3PacketDataLen) {
        if (rawProto3PacketData == nullptr || rawProto3PacketDataLen == 0)
            throw std::invalid_argument("rawProto3PacketData cannot be null or empty");

        return internalGeneratedUnwrap(rawProto3PacketData, rawProto3PacketDataLen);
    }

    /*

    Код ниже сгенерирован автоматически с помощью утилиты awd-ptrans-codegen.
    Руками не трогать. Не кормить.

     */

    // Сгенерировано с помощью awd-ptrans-codegen. (ОБЯЗАТЕЛЬНО ДОЛЖНО БЫТЬ В ОДНУ СТРОЧКУ - НИЧЕГО НЕ ПЕРЕНОСИТЬ!!!)
    std::shared_ptr<WrappedPacketData> internalGeneratedWrap(google::protobuf::Message* packet, uint32_t sequence, uint32_t ack, uint32_t ackBitfield) {
        PacketWrapper wrapper;

        wrapper.set_sequence(sequence);
        wrapper.set_ack(ack);
        wrapper.set_ack_bitfield(ackBitfield);

        if (auto* ping = dynamic_cast<Ping*>(packet)) {
            wrapper.set_allocated_ping(ping);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_ping();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* pong = dynamic_cast<Pong*>(packet)) {
            wrapper.set_allocated_pong(pong);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_pong();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* handshake_request = dynamic_cast<HandshakeRequest*>(packet)) {
            wrapper.set_allocated_handshake_request(handshake_request);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_handshake_request();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* handshake_response = dynamic_cast<HandshakeResponse*>(packet)) {
            wrapper.set_allocated_handshake_response(handshake_response);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_handshake_response();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* create_lobby_request = dynamic_cast<CreateLobbyRequest*>(packet)) {
            wrapper.set_allocated_create_lobby_request(create_lobby_request);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_create_lobby_request();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* create_lobby_response = dynamic_cast<CreateLobbyResponse*>(packet)) {
            wrapper.set_allocated_create_lobby_response(create_lobby_response);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_create_lobby_response();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* join_lobby_request = dynamic_cast<JoinLobbyRequest*>(packet)) {
            wrapper.set_allocated_join_lobby_request(join_lobby_request);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_join_lobby_request();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* join_lobby_response = dynamic_cast<JoinLobbyResponse*>(packet)) {
            wrapper.set_allocated_join_lobby_response(join_lobby_response);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_join_lobby_response();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* leave_lobby_request = dynamic_cast<LeaveLobbyRequest*>(packet)) {
            wrapper.set_allocated_leave_lobby_request(leave_lobby_request);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_leave_lobby_request();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* leave_lobby_response = dynamic_cast<LeaveLobbyResponse*>(packet)) {
            wrapper.set_allocated_leave_lobby_response(leave_lobby_response);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_leave_lobby_response();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* kicked_from_lobby = dynamic_cast<KickedFromLobby*>(packet)) {
            wrapper.set_allocated_kicked_from_lobby(kicked_from_lobby);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_kicked_from_lobby();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* updated_members_list = dynamic_cast<UpdatedMembersList*>(packet)) {
            wrapper.set_allocated_updated_members_list(updated_members_list);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_updated_members_list();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* spawn_player = dynamic_cast<SpawnPlayer*>(packet)) {
            wrapper.set_allocated_spawn_player(spawn_player);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_spawn_player();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* client_relative_move = dynamic_cast<ClientRelativeMove*>(packet)) {
            wrapper.set_allocated_client_relative_move(client_relative_move);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_client_relative_move();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else if (auto* update_entity_position = dynamic_cast<UpdateEntityPosition*>(packet)) {
            wrapper.set_allocated_update_entity_position(update_entity_position);
            size_t dataLen = wrapper.ByteSizeLong();
            std::shared_ptr<char[]> data(new char[dataLen]);
            wrapper.SerializeToArray(data.get(), static_cast<int>(dataLen));
            wrapper.release_update_entity_position();

            return std::make_shared<WrappedPacketData>(data, dataLen);
        } else
            // Код "if ..." для пакетов этого типа отсутствует выше.
            // Нужно добавить! (исп. awd-ptrans-codegen)
            throw std::invalid_argument("no implemented transformer for this packet type");
    }

    // Сгенерировано с помощью awd-ptrans-codegen. (ОБЯЗАТЕЛЬНО ДОЛЖНО БЫТЬ В ОДНУ СТРОЧКУ - НИЧЕГО НЕ ПЕРЕНОСИТЬ!!!)
    std::shared_ptr<UnwrappedPacketData> internalGeneratedUnwrap(char* data, size_t dataLen) {
        PacketWrapper wrapper;
        wrapper.ParseFromArray(data, static_cast<int>(dataLen));

        uint32_t sequence    = wrapper.sequence();
        uint32_t ack         = wrapper.ack();
        uint32_t ackBitfield = wrapper.ack_bitfield();
        
        PacketWrapper::PacketCase packetType = wrapper.packet_case();

        switch (packetType) {
            case PacketWrapper::PacketCase::kPing:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<Ping>(wrapper.ping()));

            case PacketWrapper::PacketCase::kPong:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<Pong>(wrapper.pong()));

            case PacketWrapper::PacketCase::kHandshakeRequest:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<HandshakeRequest>(wrapper.handshake_request()));

            case PacketWrapper::PacketCase::kHandshakeResponse:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<HandshakeResponse>(wrapper.handshake_response()));

            case PacketWrapper::PacketCase::kCreateLobbyRequest:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<CreateLobbyRequest>(wrapper.create_lobby_request()));

            case PacketWrapper::PacketCase::kCreateLobbyResponse:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<CreateLobbyResponse>(wrapper.create_lobby_response()));

            case PacketWrapper::PacketCase::kJoinLobbyRequest:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<JoinLobbyRequest>(wrapper.join_lobby_request()));

            case PacketWrapper::PacketCase::kJoinLobbyResponse:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<JoinLobbyResponse>(wrapper.join_lobby_response()));

            case PacketWrapper::PacketCase::kLeaveLobbyRequest:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<LeaveLobbyRequest>(wrapper.leave_lobby_request()));

            case PacketWrapper::PacketCase::kLeaveLobbyResponse:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<LeaveLobbyResponse>(wrapper.leave_lobby_response()));

            case PacketWrapper::PacketCase::kKickedFromLobby:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<KickedFromLobby>(wrapper.kicked_from_lobby()));

            case PacketWrapper::PacketCase::kUpdatedMembersList:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<UpdatedMembersList>(wrapper.updated_members_list()));

            case PacketWrapper::PacketCase::kSpawnPlayer:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<SpawnPlayer>(wrapper.spawn_player()));

            case PacketWrapper::PacketCase::kClientRelativeMove:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<ClientRelativeMove>(wrapper.client_relative_move()));

            case PacketWrapper::PacketCase::kUpdateEntityPosition:
                return std::make_shared<UnwrappedPacketData>(
                        sequence, ack, ackBitfield, packetType,
                        std::make_shared<UpdateEntityPosition>(wrapper.update_entity_position()));

            default:
                // Неизвестный пакет - он будет проигнорирован (не передан никакому PacketListener'у).
                return nullptr;
        }
    }

}
