#pragma once
#include "includes.h"
#include "ship.h"
class Player
{
private:
	double HP;
	double maxHP;

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
	void draw(sf::RenderWindow&);

	std::shared_ptr<Ship>& getShip();

	void respawn();
	void setShip(Ship);
	void setSightAngle(float);
	void subtractHP(float);
	double getPlayerHP();
	Player();
	Player(unsigned int,std::string);
	~Player();

	void printPosition();
};

