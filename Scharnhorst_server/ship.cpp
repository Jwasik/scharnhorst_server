#include "pch.h"
#include "ship.h"
#include <fstream>

#define PI 3.14159265

std::string Ship::getType()
{
	return this->type;
}

void Ship::setName(std::string name)
{
	this->name = name;
}

std::string Ship::getName()
{
	return this->name;
}

Ship::Ship()
{
	sf::ConvexShape testShape;
	testShape.setPointCount(4);
	testShape.setPoint(0, sf::Vector2f(0, 0));
	testShape.setPoint(1, sf::Vector2f(50, 0));
	testShape.setPoint(2, sf::Vector2f(50, 100));
	testShape.setPoint(3, sf::Vector2f(0, 100));
	testShape.setFillColor(sf::Color::Red);
	testShape.setOrigin(sf::Vector2f(25,50));
	this->shape = testShape;
}

Ship::Ship(std::string &name, float parameters[6], sf::ConvexShape shape)
{
	this->shape = shape;
	this->shape.move(shape.getOrigin());
	auto temporaryOrigin = shape.getOrigin();
	this->hitbox[0] = odcinek(sf::Vector2f(0,temporaryOrigin.y),sf::Vector2f(temporaryOrigin.x*2,temporaryOrigin.y));
	this->hitbox[1] = odcinek(sf::Vector2f(temporaryOrigin.y,0),sf::Vector2f(temporaryOrigin.x,temporaryOrigin.y*2));
	this->shape.move(sf::Vector2f(128, 512));
	this->shape.setFillColor(sf::Color(50, 50, 50));
	this->deleteOrigin();

	this->type = name;
	this->gear = 0;
	this->maxTurnAcceleration = parameters[0];
	this->enginePower = parameters[1];
	this->width = parameters[2];
	this->length = parameters[3];
	this->maxSpeed = parameters[4];
	this->mass = parameters[5];
	this->actualSpeed = 0;
	this->acceleration = enginePower / mass;

	shipStaticPressure = acceleration / (maxSpeed*maxSpeed);
}

Ship::Ship(const Ship &newShip)
{
	this->hitbox[0] = newShip.hitbox[0];
	this->hitbox[1] = newShip.hitbox[1];

	this->width = newShip.width;
	this->length = newShip.length;
	this->maxSpeed = newShip.length;
	this->maxTurnAcceleration = newShip.maxTurnAcceleration;
	this->turnAcceleration = newShip.turnAcceleration;
	this->mass = newShip.mass;
	this->enginePower = newShip.enginePower;
	this->force = newShip.force;
	this->waterRezistance = newShip.waterRezistance;
	this->shipStaticPressure = newShip.shipStaticPressure;
	this->gear = 0;
	this->acceleration = newShip.acceleration;
	this->actualSpeed = 0;
	this->type = newShip.type;
	this->name = newShip.name;
	for (auto & turret : newShip.turrets)
	{
		this->turrets.push_back(std::make_shared<Turret>(*turret));
	}
}


Ship::~Ship()
{
}

float Ship::calculateAcceleration()
{
	waterRezistance = shipStaticPressure * actualSpeed*actualSpeed;
	return waterRezistance;
}

void Ship::accelerate(double deltaTime) //{-1, 0, 1}
{
	calculateAcceleration();

	if (actualSpeed > 0)
	{
		actualSpeed -= waterRezistance * deltaTime;
	}
	else
	{
		actualSpeed += waterRezistance * deltaTime;
	}

	if (gear != 0)
	{
		actualSpeed += acceleration * gear*0.25 * deltaTime;
	}
}




void Ship::spin(bool direction, double dtime)
{
	turnAcceleration = maxTurnAcceleration * sin(PI*0.6*(actualSpeed / maxSpeed));
	if (direction == 1)this->rotate(dtime*turnAcceleration);
	else this->rotate(-1 * dtime*turnAcceleration);
}

void Ship::changeGear(bool change)
{
	if ((change == 0) && (gear > -1))
	{
		gear--;
	}
	else if ((change == 1) && (gear < 4))
	{
		gear++;
	}
	// w przeciwnym razie nic siê nie zmieni
}


void Ship::setCannonRotation(float angle)
{
	for (auto & turret : turrets)
	{
		turret->setRotation(angle);
	}
}

float Ship::getCannonRotation()
{
	if (turrets[0] != nullptr)return turrets[0]->getTurretAngle();
	return 0;
}

void Ship::addPoint(unsigned short number, sf::Vector2f &point)
{
	this->shape.setPoint(number, point);
}

void Ship::addTurret(std::shared_ptr<Turret> turret, sf::Vector2f positionFromShip)
{
	this->turrets.push_back(std::make_shared<Turret>(*turret));
	this->turrets.back()->setTurretPosition(positionFromShip);
}

void Ship::swim(double deltaTime)
{
	//this->accelerate(deltaTime);
	float distance = actualSpeed * deltaTime;//tutaj ta delta czasu klatki [s // poproszê w sekundach]
	this->move(sf::Vector2f(distance * sin(this->getRotation()*PI / 180), -distance * cos(this->getRotation()*PI / 180)));
	
	this->setHitboxPosition(this->shape.getPosition());


}

void Ship::setHitboxPosition(sf::Vector2f position)
{
	this->hitbox[0].setPosition(position);
	this->hitbox[1].setPosition(position);

	this->hitbox[0].updateVisual();
	this->hitbox[1].updateVisual();
}

void Ship::setTurrets(float &mouseAngle, double &dTime)
{
	auto shipPosition = this->getPosition();
	for (auto & turret : turrets)
	{
		turret->updatePosition(this->shape.getRotation(), mouseAngle, shipPosition, dTime);
	}
}

void Ship::draw(sf::RenderWindow& window)
{
	this->physical::draw(window);
	for (auto turret : turrets)
	{
		turret->draw(window);
	}
}

void Ship::shoot(std::shared_ptr<std::vector<bulletInfo>> bulletsGotFromTurret)
{
	auto angle = this->shape.getRotation();
	for (auto & turret : turrets)
	{
		turret->shoot(bulletsGotFromTurret, angle);
	}
}



