#pragma once
#include "pch.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath> //pierwiastkie i potêgi
#include <windows.h>

#define M_PI 3.14159265358979323846

static float sNR(float s);//stopnie na radiany
struct punktNaOkregu // punkt zorientowany w sposób odleg³oœæ od œrodka okrêgu: 'r', i k¹t 'a' pomiêdzy pionowym w górê promieniem 
{
	punktNaOkregu() {};
	sf::Vector2f zamienNaPunkt(sf::Vector2f orgin);
	float r;
	float a;
};

punktNaOkregu zamienNaPunktNaOkregu(sf::Vector2f punkt, sf::Vector2f srodekOkregu);

class odcinek
{
public:
	sf::Vector2f orgin;//zmieniaj aby przemieœciæ przy rotate() siê aktualizuje

	punktNaOkregu oa, ob; //i tak musz¹ byæ u¿ywane przy ka¿dej rotacji wiêc niech s¹ dostêpne tu ca³y czas 
	sf::Vector2f punkt1, punkt2; //punkty
	sf::VertexArray line; //wizualizacja
	 
	odcinek(sf::Vector2f punkt1, sf::Vector2f punkt2);
	
	float angleFromOA;
	void prosta(odcinek o, float &A, float &B, float &C); //liczy parametry równania prostej na podstawie 'odcinka'
	bool intersects(odcinek o1); //sprawdza czy odcinki siê przecinaj¹

	void rotate(float a); //obraca woko³o orgin bo woko³o czegoœ musi
	void setRotation(float a); //obraca woko³o orgin bo woko³o czegoœ musi
	void move(sf::Vector2f moveBy);//przemieszcza punkty i orgin
	void setPosition(sf::Vector2f);
	void updateVisual(); //aktualizuje line
	void setOrgin(sf::Vector2f);//ustawia orgin

	odcinek();
};