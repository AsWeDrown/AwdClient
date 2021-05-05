#define BUFFER_SIZE 1024


#include <packets.pb.h>
#include "UdpClient.hpp"
#include "PacketTransformer.hpp"
#include "../game/Game.hpp"
#include <thread>

namespace awd::net {


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PRIVATE
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    void UdpClient::startInCurrentThread() {
        udpSocket = std::make_shared<sf::UdpSocket>();

        if (udpSocket->bind(port) != sf::Socket::Done) {
            std::wcerr << L"Failed to bind UDP socket to port " << port << std::endl;
            throw std::runtime_error(FAILED_TO_BIND_UDP_SOCKET_TO_PORT);
        }

        // Чтобы можно было исправно отправлять пакеты из других потоков, пока этот поток сидит
        // и спокойно ждёт пакеты от сервера, обязательно нужно работать в non-blocking режиме.
        sf::IpAddress serverAddr = serverAddrStr;
        udpSocket->setBlocking(false);
        connected = true;

        // Начинаем с рукопожатия (Handshake).
        // Нужно для дальнейшей коммуникации с сервером.
        game::Game::instance().getNetService()->handshakeRequest();

        // Затем уже ожидаем пакеты от сервера.
        std::shared_ptr<char[]> buffer(new char[BUFFER_SIZE]);
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
                    std::wcerr << L"Ignoring empty packet." << std::endl;
                else {
                    try {
                        game::Game::instance().getPacketManager()->receivePacket(buffer, bytesReceived);
                    } catch (const std::exception& ex) {
                        // Какая-то внутренняя ошибка, пропускаем этот пакет и продолжаем работу.
                        std::wcerr << L"Failed to receive a packet "
                                      L"(internal error: " << ex.what() << L")." << std::endl;
                    }
                }
            } else if (recvStatus == sf::UdpSocket::Status::Error) {
                std::wcerr << L"UDP recv error." << std::endl;
                tearDown(); // todo RECONNECT

                break;
            }
        }
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
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

    std::string UdpClient::getServerAddrStr() const {
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
