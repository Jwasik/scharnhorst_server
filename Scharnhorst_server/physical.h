#pragma once
#include "includes.h"

#define M_PI 3.14159265358979323846

class physical
{
protected:
	
	void deleteOrigin();
public:
	sf::ConvexShape shape;

	virtual void draw(sf::RenderWindow&);
	physical();
	physical(sf::Vector2f);
	physical(sf::ConvexShape);
	void setPosition(sf::Vector2f);
	void setRotation(float);
	sf::Vector2f getPosition();
	float getRotation();
	sf::Vector2f getOrigin();

	~physical();
};

