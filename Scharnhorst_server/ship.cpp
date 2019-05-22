#include "pch.h"
#include "ship.h"

#define PI 3.14159265

std::string Ship::getType()
{
	return this->type;
}

std::string Ship::getName()
{
	return this->name;
}

void Ship::setName(std::string name)
{
	this->name = name;
}

void Ship::setShipType(std::string type)
{
	this->type = type;
}

Ship::Ship()
{
	shape.setPointCount(7);
	shape.setPoint(0, sf::Vector2f(48, 0));
	shape.setPoint(1, sf::Vector2f(16, 64));
	shape.setPoint(2, sf::Vector2f(0, 192));
	shape.setPoint(3, sf::Vector2f(16, 336));
	shape.setPoint(4, sf::Vector2f(80, 336));
	shape.setPoint(5, sf::Vector2f(96, 192));
	shape.setPoint(6, sf::Vector2f(80, 64));
	shape.setOrigin(sf::Vector2f(48,168));
	shape.move(shape.getOrigin());
	shape.move(sf::Vector2f(128,512));

	this->type = "NONE";
	this->gear = 0;
	this->maxTurnAcceleration = 8;
	this->enginePower = 117680000;
	this->width = 10;
	this->length = 10;
	this->actualSpeed = 0;
	this->maxSpeed = 16.25;
	this->mass = 38900000;
	this->acceleration = enginePower / mass;

	shipStaticPressure = acceleration/(maxSpeed*maxSpeed);

}


Ship::~Ship()
{
}


float Ship::getCannonRotation()
{
	return this->cannonRotation;
}
void Ship::setCannonRotation(float &angle)
{
	this->cannonRotation = angle;
}