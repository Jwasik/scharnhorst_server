#pragma once
#include "includes.h"
#include "movable.h"
class turret : public movable
{
protected:
	float maxLeft=0;
	float maxRight=0;
	std::string type;
	
	std::vector<sf::Vector2f> barrelPoints; //Miejsca oznaczaj�ce ko�cu luf dzia�, gdzie powinny si� pojawi� pociski
public:
	turret();
	~turret();
};

