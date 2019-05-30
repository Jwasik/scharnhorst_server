#pragma once
#include "pch.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath> //pierwiastkie i pot�gi
#include <windows.h>

#define M_PI 3.14159265358979323846

static float sNR(float s);//stopnie na radiany
struct punktNaOkregu // punkt zorientowany w spos�b odleg�o�� od �rodka okr�gu: 'r', i k�t 'a' pomi�dzy pionowym w g�r� promieniem 
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
	sf::Vector2f orgin;//zmieniaj aby przemie�ci� przy rotate() si� aktualizuje

	punktNaOkregu oa, ob; //i tak musz� by� u�ywane przy ka�dej rotacji wi�c niech s� dost�pne tu ca�y czas 
	sf::Vector2f punkt1, punkt2; //punkty
	sf::VertexArray line; //wizualizacja

	odcinek(sf::Vector2f punkt1, sf::Vector2f punkt2);
	
	void prosta(odcinek o, float &A, float &B, float &C); //liczy parametry r�wnania prostej na podstawie 'odcinka'
	bool intersects(odcinek o1); //sprawdza czy odcinki si� przecinaj�

	void rotate(float a); //obraca woko�o orgin bo woko�o czego� musi
	void setRotation(float a); //obraca woko�o orgin bo woko�o czego� musi
	void move(sf::Vector2f moveBy);//przemieszcza punkty i orgin
	void updateVisual(); //aktualizuje line
	void setOrgin(sf::Vector2f);//ustawia orgin

	odcinek();
};