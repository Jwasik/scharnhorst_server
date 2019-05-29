#pragma once
#include "pch.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath> //pierwiastkie i potêgi
#include <windows.h>
#include "Hitbox.h"

#define M_PI 3.14159265358979323846



class Hitbox // klasa opisuj¹cy obiekt namacalny wirtualnie i jego ConvexShape
{
private:
	struct odcinek;
	struct punktNaOkregu;

	sf::Vector2f RotationPoint; // wspó³rzêdne osi obrotu
	std::vector<punktNaOkregu> punkty; // dziêki nim ³¹two jest obracaæ woko³o osi hitboxa
	float a; // aktualny k¹t hitboxu 
	int ile; // ile jest punktów w polygonie
	float sNR(float s);//stopnie na radiany
	float angleRotate(float oa, float r);//sprawdza czy k¹t nie przekrêci³ 360 i sprowadza go na ziemiê 
	std::vector<odcinek> odcinki; // odcinki z jakich siê sk³ada hitbox, czyli to wodec czego bêdzie wykrywana kolizja, ich iloœæ proporcjonalnie przek³ada siê na z³o¿onoœæ obliczeniow¹ kolizji

public:
	struct odcinek
	{
		sf::Vector2f a, b; 
		sf::VertexArray line;
		odcinek(sf::Vector2f x, sf::Vector2f xx);
		void prosta(odcinek o, float* A, float* B, float* C); //liczy parametry równania prostej na podstawie 'odcinka'
		bool isCross(odcinek o1); //sprawdza czy odcinki siê przecinaj¹
		odcinek();
	};
	struct punktNaOkregu // punkt zorientowany w sposób odleg³oœæ od œrodka okrêgu: 'r', i k¹t 'a' pomiêdzy pionowym w górê promieniem 
	{
		float r;
		float a;
	};

	sf::ConvexShape polygon;// wizualna interpretacja hitboxu

	Hitbox(sf::ConvexShape polygona); // konstruktor na podstawie polygonu
	void hitboxmove(float xs, float ys);//przemieszcza wielok¹t convexShape i robi rotate o 0 co generuje nowe wartoœci odcinków na podstawie ox i oy/// move()
	void rotate(float at);//liczy nowe odcinki po obrocie na podstawie k¹tu 'a' zakualizowanego o 'at' oraz zbioru niezmiennego 'punkty' //dziedziczy z movable
	void draw(sf::RenderWindow *window); //dziedziczy z physical
	bool hit(odcinek odc);//czy którykolwiek z odcinków tworz¹cych obiekt Hitbox przecina siê z odcinkiem 'odc'
	bool objectHit(Hitbox);
	odcinek getOdcinek(int i);
	sf::Vector2f getPoint(int i);
	void setColor(sf::Color c);
	sf::Vector2f getPosition();
};