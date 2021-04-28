#define BUFFER_SIZE 51200


#include <packets.pb.h>
#include "UdpClient.hpp"
#include "PacketTransformer.hpp"
#include "../game/Game.hpp"
#include <thread>

namespace awd::net {


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *   PRIVATE
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void UdpClient::startInCurrentThread() {
        udpSocket = std::make_shared<sf::UdpSocket>();

        if (udpSocket->bind(port) != sf::Socket::Done) {
            std::string errMsg = "Failed to bind UDP socket to port " + std::to_string(port);
            std::cerr << errMsg << std::endl;

            throw std::runtime_error(errMsg);
        }

        // Чтобы можно было исправно отправлять пакеты из других потоков, пока этот поток сидит
        // и спокойно ждёт пакеты от сервера, обязательно нужно работать в non-blocking режиме.
        sf::IpAddress serverAddr = serverAddrStr;
        udpSocket->setBlocking(false);
        connected = true;

        // Начинаем с рукопожатия (Handshake).
        // Нужно для дальнейшей коммуникации с сервером.
        beginHandshake();

        // Затем уже ожидаем пакеты от сервера.
        std::shared_ptr<char[]> buffer(new char[51200]);
        std::size_t bytesReceived;

        while (connected) {
            // Копии делать ОБЯЗАТЕЛЬНО. В противном случае пакеты просто не будут отправляться.
            // Это из-за того, что SFML изменяет значения передаваемых ему по ссылке значений
            // IP-адреса и порта, в том числе если никаких пакетов получено не было (например,
            // статус NotReady в non-blocking режиме).
            sf::IpAddress serverAddrCopy = serverAddr;
            unsigned short portCopy = port;

            auto recvStatus = udpSocket->receive(
                    buffer.get(), BUFFER_SIZE,bytesReceived, serverAddrCopy, portCopy);

            if (recvStatus == sf::UdpSocket::Status::Done) {
                if (bytesReceived == 0)
                    std::cerr << "Ignoring empty packet." << std::endl;
                else {
                    auto unwrappedPacketData = unwrap(buffer.get(), bytesReceived);
                    game::Game::instance().getPacketManager()->receivePacket(
                            unwrappedPacketData->getPacketType(), unwrappedPacketData->getPacket());
                }
            } else if (recvStatus == sf::UdpSocket::Status::Error) {
                std::cerr << "UDP recv error" << std::endl;
                tearDown(); // todo RECONNECT

                break;
            }
        }
    }

    void UdpClient::beginHandshake() {
        auto* handshakeRequest = new HandshakeRequest();
        handshakeRequest->set_protocol_version(PacketManager::PROTOCOL_VERSION);
        game::Game::instance().getPacketManager()->sendPacket(handshakeRequest);
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *   PUBLIC
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    UdpClient::UdpClient(const std::string& serverAddrStr, unsigned short port) {
        this->serverAddrStr = serverAddrStr;
        this->port = port;
    }

    void UdpClient::startInNewThread() {
        connected = true;
        std::thread socketThread(&UdpClient::startInCurrentThread, this);
        socketThread.detach(); // отправляем поток в свободное плавание
    }

    void UdpClient::tearDown() {
        connected = false;
        udpSocket->unbind();
        udpSocket = nullptr;
    }

    const std::string& UdpClient::getServerAddrStr() const {
        return serverAddrStr;
    }

    unsigned short UdpClient::getPort() const {
        return port;
    }

    bool UdpClient::isConnected() const {
        return connected;
    }

    std::shared_ptr<sf::UdpSocket> UdpClient::getUdpSocket() const {
        return udpSocket;
    }

}
