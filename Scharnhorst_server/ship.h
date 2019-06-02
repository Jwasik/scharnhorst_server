#pragma once
#include "includes.h"
#include "movable.h"
#include "turret.h"
#include "odcinek.h"



class Ship : public movable
{
public:

	float width; // [m] 
	float length; // [m]
	float maxSpeed; // [m/s]
	float maxTurnAcceleration;
	float turnAcceleration; // [stopni/sek]
	float mass; // [kg]
	float enginePower; // [Wat]
	float force;// zale�na od waterResistance
	float waterRezistance; // zale�na od pr�dko�ci 
	float shipStaticPressure; // sta�a op�ywowo�ci kad�uba
	int gear; //biegi -1 - 4
	float acceleration; // [m/s^2]
	float actualSpeed;// [m/s]	
	std::string type;
	std::string name;
	float calculateAcceleration();
	std::vector<std::shared_ptr<Turret>> turrets;

	odcinek hitbox[2];
	void draw(sf::RenderWindow&);
	void accelerate(double);
	void swim(double); //przes�wa i obraca statek raz na klatk� oraz go rysuje(dzi�ki physical::draw kt�re w przysz�o�ci zostanie zmieniona na bitmap�) 
	void spin(bool, double); // natychmiastowy obr�t o akkcelerancj� kontow� w czasie
	void changeGear(bool); // zmana biegu 0 dla -- 1 dla ++
	void setCannonRotation(float);
	void setTurrets(float &, double &);
	float getCannonRotation();
	void addPoint(unsigned short, sf::Vector2f&);
	void addTurret(std::shared_ptr<Turret>, sf::Vector2f);
	void shoot(std::shared_ptr<std::vector<bulletInfo>>);

	std::string getType();
	void setName(std::string);
	std::string getName();
	Ship();
	Ship(std::string&, float[6], sf::ConvexShape);
	Ship(const Ship&);
	~Ship();
};

