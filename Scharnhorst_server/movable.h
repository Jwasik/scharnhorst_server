#pragma once
#include "physical.h"
#include "Hitbox.h"
class movable : public physical //,public sf::Drawable
{
protected:
public:
	float changeAngle(float, float);
	float changeAngle(float, float, float);
	float stopnieNaRadiany(float s);
	float howManyDegreeFrom(float, float);
	movable();
	movable(sf::Vector2f);
	void move(sf::Vector2f);
	void rotate(float);
	Hitbox::punktNaOkregu zamienNaPunktNaOkregu(sf::Vector2f punkt, sf::Vector2f srodekOkregu);
	~movable();
};

