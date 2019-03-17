#include "pch.h"
#include "client.h"


Client::Client()
{
	this->UdpPort = 0;
}


Client::~Client()
{
}

sf::TcpSocket & Client::getTcpSocket()
{
	return this->orderSocket;
}

sf::TcpSocket::Status Client::sendTcp(sf::Packet packet)
{
	return orderSocket.send(packet);
}
sf::TcpSocket::Status Client::sendUdp(sf::Packet packet)
{
	if(UdpPort != 0)return outSocket.send(packet,orderSocket.getRemoteAddress(),UdpPort);
	else return sf::TcpSocket::Status::Error;
}

sf::TcpSocket::Status Client::receiveTcp(sf::Packet &packet)
{
	return orderSocket.receive(packet);
}

sf::IpAddress Client::getRemoteAddress()
{
	return orderSocket.getRemoteAddress();
}

unsigned int Client::getOutUdpPort()
{
	return this->UdpPort;
}

void Client::setBlocking(bool status)
{
	orderSocket.setBlocking(status);
}

void Client::setOutUdpPort(unsigned int port)
{
	this->UdpPort = port;
}
