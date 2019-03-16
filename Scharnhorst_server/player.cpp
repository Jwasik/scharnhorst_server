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

std::shared_ptr<Ship> & Player::getShip()
{
	return this->playerShip;
}

Player::Player()
{
	playerShip = std::make_shared<Ship>();
}

Player::Player(std::string playerName, sf::TcpSocket &socket)
{
	playerShip = std::make_shared<Ship>();
	this->playerName = playerName;
	sf::Packet packet;
	packet << "PLA" << 0 << playerName << this->getShip()->getType();
}


Player::~Player()
{
}
