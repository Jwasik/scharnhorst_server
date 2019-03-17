#pragma once
#include<SFML/Network.hpp>
#include<iostream>

class Client
{
private:
	unsigned int UdpPort;
	sf::TcpSocket orderSocket;
	sf::UdpSocket outSocket;
public:
	Client();
	~Client();
	sf::TcpSocket& getTcpSocket();
	sf::TcpSocket::Status sendTcp( sf::Packet);
	sf::TcpSocket::Status sendUdp( sf::Packet);
	sf::TcpSocket::Status receiveTcp( sf::Packet&);

	sf::IpAddress getRemoteAddress();

	void setBlocking(bool);
	void setOutUdpPort(unsigned int);
	unsigned int getOutUdpPort();

};

