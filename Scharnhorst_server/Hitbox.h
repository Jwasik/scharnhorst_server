#pragma once
#include "pch.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath> //pierwiastkie i pot�gi
#include <windows.h>
#include "Hitbox.h"

#define M_PI 3.14159265358979323846



class Hitbox // klasa opisuj�cy obiekt namacalny wirtualnie i jego ConvexShape
{
private:
	struct odcinek;
	struct punktNaOkregu;

	sf::Vector2f RotationPoint; // wsp�rz�dne osi obrotu
	std::vector<punktNaOkregu> punkty; // dzi�ki nim ��two jest obraca� woko�o osi hitboxa
	float a; // aktualny k�t hitboxu 
	int ile; // ile jest punkt�w w polygonie
	float sNR(float s);//stopnie na radiany
	float angleRotate(float oa, float r);//sprawdza czy k�t nie przekr�ci� 360 i sprowadza go na ziemi� 
	std::vector<odcinek> odcinki; // odcinki z jakich si� sk�ada hitbox, czyli to wodec czego b�dzie wykrywana kolizja, ich ilo�� proporcjonalnie przek�ada si� na z�o�ono�� obliczeniow� kolizji

public:
	struct odcinek
	{

		sf::Vector2f a, b; 
		sf::VertexArray line;
		odcinek(sf::Vector2f x, sf::Vector2f xx);
		void prosta(odcinek o, float* A, float* B, float* C); //liczy parametry r�wnania prostej na podstawie 'odcinka'
		bool isCross(odcinek o1); //sprawdza czy odcinki si� przecinaj�
		odcinek();


	};
	struct punktNaOkregu // punkt zorientowany w spos�b odleg�o�� od �rodka okr�gu: 'r', i k�t 'a' pomi�dzy pionowym w g�r� promieniem 
	{
		float r;
		float a;
	};

	sf::ConvexShape polygon;// wizualna interpretacja hitboxu

	Hitbox(sf::ConvexShape polygona); // konstruktor na podstawie polygonu
	void hitboxmove(float xs, float ys);//przemieszcza wielok�t convexShape i robi rotate o 0 co generuje nowe warto�ci odcink�w na podstawie ox i oy/// move()
	void rotate(float at);//liczy nowe odcinki po obrocie na podstawie k�tu 'a' zakualizowanego o 'at' oraz zbioru niezmiennego 'punkty' //dziedziczy z movable
	void draw(sf::RenderWindow *window); //dziedziczy z physical
	bool hit(odcinek odc);//czy kt�rykolwiek z odcink�w tworz�cych obiekt Hitbox przecina si� z odcinkiem 'odc'
	bool objectHit(Hitbox);
	odcinek getOdcinek(int i);
	sf::Vector2f getPoint(int i);
	void setColor(sf::Color c);
	sf::Vector2f getPosition();
};