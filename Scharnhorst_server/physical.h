#pragma once
#include "includes.h"

#define M_PI 3.14159265358979323846

class physical
{

protected:


public:
	sf::ConvexShape shape;

	physical();
	physical(sf::Vector2f);
	void setPosition(sf::Vector2f);
	void setRotation(float);
	sf::Vector2f getPosition();
	float getRotation();


	~physical();

};

