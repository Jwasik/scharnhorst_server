#pragma once
#include "includes.h"
#include "movable.h"
class Bullet : public movable
{
protected:
	std::string type;
	float speed;
	float damage;
	float angle;
	sf::Vector2f movementVector;//przechowuje wektor przesuniêcia z altualn¹ prêdkoœci¹ i k¹tem w czasie 1 sekundy

public:
	Hitbox::odcinek tracer;

	Bullet();
	Bullet(const Bullet&);
	Bullet(std::string type, sf::ConvexShape body, float speed, float damage, float angle, sf::Vector2f punkt);
	Bullet(std::string, sf::ConvexShape, float, float);

	void calculateMovementVector();
	void fly(double deltaTime);
	void draw(sf::RenderWindow&);
	void setBulletInfo(const jw::bulletInfo&);
	std::string getType();

	~Bullet();
};

