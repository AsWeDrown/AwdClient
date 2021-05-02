#pragma once


#include <SFML/Network/UdpSocket.hpp>
#include <memory>
#include <atomic>

namespace awd::net {

    class UdpClient {
    private:
        std::atomic<bool> connected = false;
        std::string serverAddrStr;
        unsigned short port;
        std::shared_ptr<sf::UdpSocket> udpSocket = nullptr;

        void startInCurrentThread();
        void beginHandshake();

    public:
        UdpClient(const std::string& serverAddr, unsigned short port);

        void startInNewThread();
        void tearDown();

        std::string getServerAddrStr() const;
        unsigned short getPort() const;
        bool isConnected() const;
        std::shared_ptr<sf::UdpSocket> getUdpSocket() const;
    };

}
