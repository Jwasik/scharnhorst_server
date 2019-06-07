#pragma once
#include "includes.h"
#include "functions.h"
#include "odcinek.h"
#include "movable.h"



class Bullet : public movable
{
protected:
	std::string type;
	float speed;
	float damage;
	float angle;
	float caliber;
	sf::Vector2f movementVector;//przechowuje wektor przesuni�cia z altualn� pr�dko�ci� i k�tem w czasie 1 sekundy

public:
	odcinek tracer;

	unsigned int ownerId;
	unsigned int bulletId;

	Bullet();
	Bullet(const Bullet&);
	Bullet(std::string type, sf::ConvexShape body, float speed, float damage, float angle, int caliber, sf::Vector2f punkt);
	Bullet(std::string, sf::ConvexShape, float, float,int);

	void calculateMovementVector();
	void fly(double deltaTime);
	void draw(sf::RenderWindow&);
	void setBulletInfo(const bulletInfo&);
	float getDamage();
	std::string getType();

	~Bullet();
};

sf::Packet& operator<<(sf::Packet&, Bullet&);