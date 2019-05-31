#pragma once
#include<SFML/Network.hpp>
#include<iostream>

class Client
{
private:
	sf::Clock connectionClock;
	unsigned short clientUdpPort;
	sf::IpAddress clientAddress;
	sf::TcpSocket orderSocket;
	sf::UdpSocket outSocket;
	unsigned int playerId;
public:
	Client();
	~Client();
	sf::TcpSocket& getTcpSocket();
	sf::TcpSocket::Status sendTcp( sf::Packet);
	sf::TcpSocket::Status sendUdp( sf::Packet);
	sf::TcpSocket::Status receiveTcp( sf::Packet&);

	sf::IpAddress getRemoteAddress();

	void resetConnectionClock();
	void setBlocking(bool);
	void setOutUdpPort(unsigned int);
	void setPlayerId(unsigned int);
	unsigned int getOutUdpPort();
	unsigned int getPlayerId();
	sf::Time getTimeFromLastActivity();

};

