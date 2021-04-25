#pragma once

#define UDP_PORT 23132


#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <memory>
#include "packets.pb.h"
#include "ptrans.hpp"

namespace awd::net {

    class Client {
    private:
        sf::UdpSocket socket;

    public:
        Client() {
            short unsigned int port = UDP_PORT;

            if (socket.bind(port) != sf::Socket::Done)
                throw std::runtime_error("failed to start a AWD client: bind failure");

            sf::IpAddress recipient = "julia.reflex.rip";
            auto* ka = new awd::net::KeepAlive();
            ka->set_playerid(987654321);
            ka->set_testid(-228000);
            std::shared_ptr<WrappedPacketData> wrappedka = wrap(ka);

            if (socket.send(wrappedka->getData().get(),
                            wrappedka->getDataLen(), recipient, port) != sf::Socket::Done)
                throw std::runtime_error("failed to send packet");

            std::cout << "Sent!" << std::endl;
            std::cout << "receiving.." << std::endl;

            sf::IpAddress sender = "julia.reflex.rip";
            std::shared_ptr<char[]> recdata(new char[51200]);
            std::size_t received;

            if (socket.receive(recdata.get(), 51200, received, sender, port) != sf::Socket::Done)
                throw std::runtime_error("failed to receive packet");

            std::cout << "Received " << received << " bytes from " << sender << " on port " << port << std::endl;

            if (received > 0) {
                std::cout << "Received data: ";

                for (int i = 0; i < received; i++)
                    std::cout << ((int) recdata[i]) << " ";

                std::cout << std::endl;
            }

            std::shared_ptr<UnwrappedPacketData> unwrappedPacketData = unwrap(recdata.get(), received);

            if (unwrappedPacketData->getPacketType() == PacketWrapper::PacketCase::kKeepAlive) {
                auto recpacket = unwrappedPacketData->getPacket();
                auto recka = std::dynamic_pointer_cast<KeepAlive>(recpacket);
                std::cout << "received expected KeepAlive: " << unwrappedPacketData->getPacketType() << std::endl;;
                std::cout << "(recka begin)" << std::endl;
                std::cout << "    playerId = " << recka->playerid() << std::endl;
                std::cout << "    testId = " << recka->testid() << std::endl;
                std::cout << "(recka end)" << std::endl;
            } else
                std::cout << "unexpected packet (debug!): " << unwrappedPacketData->getPacketType() << std::endl;
        }
    };

}