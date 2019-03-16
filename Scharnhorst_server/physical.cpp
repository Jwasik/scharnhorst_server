#include "pch.h"
#include "physical.h"


physical::physical()
{
	setPosition(sf::Vector2f(0,0));
}

physical::physical(sf::Vector2f position)
{
	setPosition(position);
}

void physical::setPosition(sf::Vector2f position)
{
	this->shape.setPosition(position);
}

void physical::setRotation(float alpha)
{
	this->shape.setRotation(alpha);
}
sf::Vector2f physical::getPosition()
{
	return shape.getPosition();
}

float physical::getRotation()
{
	return shape.getRotation();
}

physical::~physical()
{
}
