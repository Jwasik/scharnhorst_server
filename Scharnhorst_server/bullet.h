#pragma once
#include "includes.h"
#include "movable.h"
class Bullet : public movable
{
protected:
	std::string type;
	sf::ConvexShape body;
	float speed;
	float damage;
	float angle;

	void loadBulletType(std::string);

public:
	Bullet();
	Bullet(sf::Vector2f, float, std::string);
	~Bullet();
};

