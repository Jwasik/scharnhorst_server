#include "pch.h"
#include "bullet.h"


sf::Packet & operator<<(sf::Packet& packet, Bullet& bullet)
{
	packet << bullet.bulletId;
	packet << bullet.ownerId;
	packet << bullet.getDamage();
	return packet;
}

void Bullet::calculateMovementVector()
{
	movementVector.x = sin(stopnieNaRadiany(this->angle));
	movementVector.y = cos(stopnieNaRadiany(this->angle));
}

void Bullet::fly(double deltaTime)
{
	float distance = speed * deltaTime * 8;
	this->shape.setPosition(this->shape.getPosition() + sf::Vector2f(distance * movementVector.x, -distance * movementVector.y));

	this->tracer = odcinek(this->tracer.punkt2, this->shape.getPosition());
}

void Bullet::draw(sf::RenderWindow& window)
{
	window.draw(this->tracer.line);
	window.draw(this->shape);
}

void Bullet::setBulletInfo(const bulletInfo &info)
{
	this->type = info.name;
	this->setPosition(info.position);
	this->angle = info.angle;
	this->shape.setRotation(info.angle);
	this->tracer.punkt1 = info.position;
	this->tracer.punkt2 = info.position;
	this->ownerId = info.ownerId;
	this->bulletId = info.bulletId;
}

float Bullet::getDamage()
{
	return this->damage;
}

std::string Bullet::getType()
{
	return this->type;
}

Bullet::Bullet(std::string ntype, sf::ConvexShape nbody, float nspeed, float ndamage, float nangle, sf::Vector2f punkt) : type(ntype), speed(nspeed), damage(ndamage), angle(nangle)
{
	this->shape = nbody;
	this->tracer.punkt2 = punkt;
	this->shape.setRotation(angle);
	this->shape.setPosition(tracer.punkt2);
	this->calculateMovementVector();
	this->movementVector = sf::Vector2f(10, 10);//dla testów
}

Bullet::Bullet(std::string name, sf::ConvexShape body, float speed, float damage)
{
	this->type = name;
	this->shape = body;
	this->speed = speed;
	this->damage = damage;
	this->calculateMovementVector();
	this->movementVector = sf::Vector2f(10, 10);//dla testów
}

Bullet::Bullet()
{
}

Bullet::Bullet(const Bullet &origin)
{
	this->type = origin.type;
	this->speed = origin.speed;
	this->damage = origin.damage;
	this->angle = origin.angle;
	this->shape = origin.shape;
	this->tracer = origin.tracer;
	this->ownerId = origin.ownerId;
	this->bulletId = origin.bulletId;
	this->calculateMovementVector();
}


Bullet::~Bullet()
{
}
