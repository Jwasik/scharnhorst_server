#pragma once
#include "includes.h"
#include "bullet.h"
#include "movable.h"
#include "odcinek.h"

struct Barrel : public movable
{

public:
	unsigned int barrelSize;
	float length;//D³ugoœæ lufy
	std::shared_ptr<Bullet> mainBulletType;
	std::string name;
	punktNaOkregu punkt;
	Barrel();
	Barrel(std::string name, sf::Vector2f point);
	Barrel(std::string name, sf::Vector2f point, sf::ConvexShape shape, Bullet, unsigned int);
	void updatePosition(float TurretAngle, sf::Vector2f TurretOrigin);
};

class Turret : public movable
{
protected:

	std::string type;
	std::string name;

	sf::Vector2f shipOrigin;
	sf::Vector2f position;
	std::vector< std::shared_ptr<Barrel>> barrels; //Miejsca oznaczaj¹ce koñcu luf dzia³, gdzie powinny siê pojawiæ pociski

	float rotationSpeed; //w stopniach/sekundê
	float distanceFromShipOrigin;
	float angleFromShipOrigin;
	float shipAngle;
	float restrictedArea[2];//k¹towy obszar w którym Turret nie mo¿e przebywaæ (mniejszy k¹t, wiêkszy k¹t) 
	float arestrictedArea[2];
	float turretAngle;
	bool turretCanDo360;
	int middleOfLockedArea;


public:
	Turret();
	Turret(const Turret & Turret);
	Turret(std::string ntype, float ndistanceFromShipOrigin, float nangleFromShipOrigin, std::vector<std::shared_ptr<Barrel>> nbarrels);
	Turret(std::string ntype, std::string nname, sf::ConvexShape turretBody, float parameters[3]);
	~Turret();



	void draw(sf::RenderWindow& window);
	void updatePosition(float nshipAngle, float nTurretAngle, sf::Vector2f nshipOrigin, float dTime);

	std::vector<std::shared_ptr<sf::Vector2f>> getBarrelsPositionsByWater();
	void updateRestrictedAreaBy(float moveRestricted);
	float getShipAngle();
	float getAngleByWater();
	float getTurretAngle();
	void shoot(std::shared_ptr<std::vector<jw::bulletInfo>>, float&);
	void addPoint(int number, sf::Vector2f point);
	void addBarrel(Barrel, sf::Vector2f);
	void setTurretPosition(sf::Vector2f);
	void setRestrictedArea(float[2]);
};

