#include "pch.h"
#include "bullet.h"

void Bullet::calculateMovementVector()
{
	movementVector.x = sin(stopnieNaRadiany(this->angle));
	movementVector.y = cos(stopnieNaRadiany(this->angle));
}

void Bullet::fly(double deltaTime)
{
	float distance = speed * deltaTime;
	this->shape.setPosition(this->shape.getPosition() + sf::Vector2f(distance * movementVector.x, -distance * movementVector.y));

	this->tracer = Hitbox::odcinek(this->tracer.b, this->shape.getPosition());
}

void Bullet::draw(sf::RenderWindow& window)
{
	window.draw(this->tracer.line);
	window.draw(this->shape);
}

void Bullet::setBulletInfo(const jw::bulletInfo &info)
{
	this->type = info.name;
	this->setPosition(info.position);
	this->angle = info.angle;
	this->shape.setRotation(info.angle);
	this->tracer.a = info.position;
	this->tracer.b = info.position;
}

std::string Bullet::getType()
{
	return this->type;
}

Bullet::Bullet(std::string ntype, sf::ConvexShape nbody, float nspeed, float ndamage, float nangle, sf::Vector2f punkt) : type(ntype), speed(nspeed), damage(ndamage), angle(nangle)
{
	this->shape = nbody;
	this->tracer.b = punkt;
	this->shape.setRotation(angle);
	this->shape.setPosition(tracer.b);
	this->calculateMovementVector();
	this->movementVector = sf::Vector2f(10, 10);//dla test�w
}

Bullet::Bullet(std::string name, sf::ConvexShape body, float speed, float damage)
{
	this->type = name;
	this->shape = body;
	this->speed = speed;
	this->damage = damage;
	this->calculateMovementVector();
	this->movementVector = sf::Vector2f(10, 10);//dla test�w
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
	this->calculateMovementVector();
}


Bullet::~Bullet()
{
}
