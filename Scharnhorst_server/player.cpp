#include "pch.h"
#include "player.h"


unsigned int Player::getPlayerId()
{
	return this->playerId;
}

void Player::setId(unsigned int newId)
{
	this->playerId = newId;
}



void Player::doStuff(double &deltaTime)
{
	if (this->playerShip != nullptr)
	{
		playerShip->swim(deltaTime);
	}
}

void Player::sendPlayerPosition(sf::UdpSocket &socket, sf::IpAddress &address, unsigned short &port)
{
	sf::Packet packet;
	packet << "POS" << this->playerId << this->getShip()->getPosition().x << this->getShip()->getPosition().y << this->sightAngle; 
	socket.send(packet,address,port);
}

void Player::draw(sf::RenderWindow &window)
{
	this->playerShip->draw(window);
}

sf::Packet Player::preparePOSpacket()
{
	sf::Packet sendingPacket;
	sendingPacket.clear();
	sendingPacket << "POS";
	sendingPacket << this->playerId;
	sendingPacket << this->getShip()->getPosition().x;
	sendingPacket << this->getShip()->getPosition().y;
	sendingPacket << this->getShip()->getRotation();
	sendingPacket << this->getShip()->getCannonRotation();

	return sendingPacket;
}

sf::Packet Player::preparePLApacket()
{
	sf::Packet PLApacket;
	PLApacket.clear();
	PLApacket << "PLA";
	PLApacket << this->playerId;
	PLApacket << this->playerName;
	PLApacket << this->getShip()->getType();
	return PLApacket;
}

std::shared_ptr<Ship> & Player::getShip()
{
	return this->playerShip;
}

Player::Player()
{
	playerShip = std::make_shared<Ship>();
}

Player::Player(unsigned int id,std::string playerName)
{
	playerShip = std::make_shared<Ship>();
	this->playerId = id;
	this->playerName = playerName;
}


Player::~Player()
{
}

void Player::printPosition()
{
	std::cout << "Player " << this->playerName << " position " << this->getShip()->getPosition().x << ' ' <<  this->getShip()->getPosition().y << std::endl;
}
