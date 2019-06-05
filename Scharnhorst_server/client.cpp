#include "pch.h"
#include "client.h"


Client::Client()
{
	this->connectionClock.restart();
	this->clientUdpPort = 0;
}


Client::~Client()
{
}

sf::TcpSocket & Client::getTcpSocket()
{
	return this->orderSocket;
}

sf::Socket::Status Client::sendTcp(sf::Packet packet)
{
	return orderSocket.send(packet);
}
sf::Socket::Status Client::sendUdp(sf::Packet packet)
{
	outSocket.setBlocking(true);
	outSocket.send(packet,orderSocket.getRemoteAddress(),clientUdpPort);
	outSocket.setBlocking(false);
	return sf::Socket::Status::Done;
}

sf::Socket::Status Client::receiveTcp(sf::Packet &packet)
{
	return orderSocket.receive(packet);
}

sf::IpAddress Client::getRemoteAddress()
{
	return orderSocket.getRemoteAddress();
}

unsigned int Client::getOutUdpPort()
{
	return this->clientUdpPort;
}

unsigned int Client::getPlayerId()
{
	return this->playerId;
}

sf::Time Client::getTimeFromLastActivity()
{
	return this->connectionClock.getElapsedTime();
}

void Client::resetConnectionClock()
{
	this->connectionClock.restart();
}

void Client::setBlocking(bool status)
{
	orderSocket.setBlocking(status);
}

void Client::setOutUdpPort(unsigned int port)
{
	this->clientAddress = orderSocket.getRemoteAddress();
	this->clientUdpPort = port;
}

void Client::setPlayerId(unsigned int id)
{
	this->playerId = id;
}
