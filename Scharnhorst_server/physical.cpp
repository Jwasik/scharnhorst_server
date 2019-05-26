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

physical::physical(sf::ConvexShape nshape) : shape(nshape)
{
	;
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

sf::Vector2f physical::getOrigin()
{
	return this->shape.getOrigin();
}

void physical::draw(sf::RenderWindow &window)
{
	window.draw(this->shape);
}

void physical::deleteOrigin()
{
	for (int i = 0; i < shape.getPointCount(); i++)
	{
		shape.setPoint(i, shape.getPoint(i) - shape.getOrigin());
	}
	shape.setOrigin(sf::Vector2f(0, 0));
}

physical::~physical()
{
}

