#pragma once
#include "includes.h"
#include "movable.h"
class turret : public movable
{
protected:
	float maxLeft=0;
	float maxRight=0;
	std::string type;
	
	std::vector<sf::Vector2f> barrelPoints; //Miejsca oznaczaj¹ce koñcu luf dzia³, gdzie powinny siê pojawiæ pociski
public:
	turret();
	~turret();
};

