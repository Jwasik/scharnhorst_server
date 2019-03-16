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
	void doStuff(double&);
	void sendPlayerPosition(sf::UdpSocket&, sf::IpAddress&, unsigned short&);
	void draw(sf::RenderWindow&);

	std::shared_ptr<Ship>& getShip();
	Player();
	Player(std::string, sf::TcpSocket&);
	~Player();
};

