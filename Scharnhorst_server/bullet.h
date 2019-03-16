#pragma once
#include "includes.h"
#include "movable.h"
class bullet : public movable
{
protected:
	std::string type;
	sf::ConvexShape body;
	float speed;
	float damage;
	float angle;

public:
	bullet();
	~bullet();
};

