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
        // Начинаем с рукопожатия (Handshake).
        // Нужно для дальнейшей коммуникации с сервером.
        std::wcerr << L"creating socket." << std::endl;
        udpSocket = std::make_shared<sf::UdpSocket>();

        // Затем уже ожидаем пакеты от сервера.
        auto status = udpSocket->bind(port);

        if (status != sf::Socket::Done) {
            game::Game::instance().socketBindFailed();
            return;
        }

        std::wcout << L"Bind status: " << status << std::endl;

        std::wcerr << L"Handshaking..." << std::endl;
        game::Game::instance().getNetService()->handshakeRequest();
        std::wcerr << L"Binding UDP..." << std::endl;

        // Чтобы можно было исправно отправлять пакеты из других потоков, пока этот поток сидит
        // и спокойно ждёт пакеты от сервера, обязательно нужно работать в non-blocking режиме.
        sf::IpAddress serverAddr = serverAddrStr;
        udpSocket->setBlocking(false);
        connected = true;

        std::shared_ptr<char[]> buffer(new char[BUFFER_SIZE]);
        std::size_t bytesReceived;

        std::wcout << L"Listening.................." << std::endl;

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
                        auto packetData = game::Game::instance()
                                .getPacketManager()->receivePacket(buffer, bytesReceived);

                        if (packetData != nullptr)
                            game::Game::instance().getNetService()->enqueueReceive(packetData);
                        else
                            std::wcerr << L"Ignoring unknown packet (failed to unwrap)." << std::endl;
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

        std::wcout << L"-- END OF UDP CLIENT LOOP --" << std::endl;
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
        // Создаём клиент и начинаем "рукопожатие" (handshake).
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
