#pragma once
#include "includes.h"


	//TCPConnect(sf::socket,std::string adres) ³¹czy socket z podanym adresem.


	//Wypisuje na ekran podany sf::status
	void printStatus(const sf::Socket::Status&);
	void printPOSPacket(sf::Packet);

	struct bulletInfo
	{
		std::string name;
		sf::Vector2f position;
		float angle;
		unsigned int ownerId;
		unsigned int bulletId;
	};

sf::Packet operator<<(sf::Packet&, const bulletInfo&);
sf::Packet& operator>>(sf::Packet&, bulletInfo&);


