#pragma once
#include "includes.h"
#include "movable.h"
class Ship : public movable
{
private:

public:
	float width; // [m]
	float length; // [m]
	float maxSpeed; // [m/s]
	float maxTurnAcceleration;
	float turnAcceleration; // [stopni/sek]
	float mass; // [kg]
	float enginePower; // [Wat]
	float force;// zale¿na od waterResistance
	float waterRezistance; // zale¿na od prêdkoœci 
	float shipStaticPressure; // sta³a op³ywowoœci kad³uba
	int gear; //biegi -1 - 4
	float acceleration; // [m/s^2]
	float actualSpeed;// [m/s]	
	std::string type;
	std::string name;

	float calculateAcceleration();


	void accelerate(double);
	void swim(double); //przesówa i obraca statek raz na klatkê oraz go rysuje(dziêki physical::draw które w przysz³oœci zostanie zmieniona na bitmapê) 
	void spin(bool, double); // natychmiastowy obrót o akkcelerancjê kontow¹ w czasie
	void changeGear(bool); // zmana biegu 0 dla -- 1 dla ++
	void draw(sf::RenderWindow&);
	void setCannonRotation(float);
	float getCannonRotation();

	std::string getType();
	void setShipType(std::string);
	Ship();
	~Ship();
};

