#pragma once
#include "includes.h"
#include "ship.h"
class Player
{
private:
	float sightAngle = 0;
	std::string playerName;
	unsigned int playerId;

public:
	std::shared_ptr<Ship> playerShip;

	unsigned int getPlayerId();
	void setId(unsigned int);
	void sendPlayerPosition(sf::UdpSocket&, sf::IpAddress&, unsigned short&);
	sf::Packet preparePOSpacket();
	sf::Packet preparePLApacket();

	std::shared_ptr<Ship>& getShip();
	void setShip(Ship);
	void setSightAngle(float);

	Player();
	Player(unsigned int,std::string);
	~Player();

	void printPosition();
};

