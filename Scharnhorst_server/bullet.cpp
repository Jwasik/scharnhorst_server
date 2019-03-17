#include "pch.h"
#include "bullet.h"


void Bullet::loadBulletType(std::string)
{
}

Bullet::Bullet()
{
}

Bullet::Bullet(sf::Vector2f position, float angle, std::string type = "NULL_BULLET")
{
	this->setPosition(position);
	this->setRotation(angle);
	this->loadBulletType(type);
}


Bullet::~Bullet()
{
}
