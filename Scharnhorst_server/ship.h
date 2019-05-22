#pragma once
#include "includes.h"
#include "movable.h"
class Ship : public movable
{
private:
	float cannonRotation;
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

	float getCannonRotation();
	void setCannonRotation(float & angle);

	std::string getType();
	std::string getName();
	void setName(std::string);
	void setShipType(std::string);
	Ship();
	~Ship();
};

