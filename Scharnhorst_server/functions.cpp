#include "pch.h"
#include "functions.h"





void printStatus(const sf::Socket::Status &status)
{
	switch (status)
	{
	case sf::Socket::Status::Error:
			std::cout << "connection error" << std::endl;
			break;
	case sf::Socket::Status::Done:
		std::cout << "connection done" << std::endl;
		break;
	case sf::Socket::Status::NotReady:
		std::cout << "connection not ready" << std::endl;
		break;
	case sf::Socket::Status::Disconnected:
		std::cout << "connection disconnected" << std::endl;
		break;
	case sf::Socket::Status::Partial:
		std::cout << "connection partial done" << std::endl;
		break;
		default:
			std::cout << "unknown connection status" << std::endl;

	}
}

void printPOSPacket(sf::Packet packet)
{
	std::cout << packet.getDataSize() << ' ';
	std::string type = "ERR";
	unsigned int id = 0;
	float x, y, angle, cannonAngle;
	x = y = angle = cannonAngle = 420;
	packet >> type;
	packet >> id;
	packet >> x, y, angle, cannonAngle;
	std::cout << "packet " << type << ' ' << id << ' ' << x << ' ' << y << ' ' << angle << ' ' << cannonAngle << std::endl;
}

sf::Packet operator<<(sf::Packet & packet, const bulletInfo & info)
{
	packet << info.name;
	packet << info.position.x;
	packet << info.position.y;
	packet << info.angle;
	packet << info.ownerId;
	packet << info.bulletId;
	return packet;
}

sf::Packet& operator>>(sf::Packet &packet, bulletInfo &info)
{
	packet >> info.name;
	packet >> info.position.x;
	packet >> info.position.y;
	packet >> info.angle;
	packet >> info.ownerId;
	packet >> info.bulletId;

	return packet;
}
