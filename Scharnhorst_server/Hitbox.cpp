#include "pch.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath> //pierwiastkie i potêgi
#include <windows.h>
#include "Hitbox.h"

#define M_PI 3.14159265358979323846

Hitbox::odcinek::odcinek(sf::Vector2f x, sf::Vector2f xx) : a(x), b(xx) 
{
	line = sf::VertexArray(sf::LineStrip, 2);
	line[0].position = a;
	line[1].position = b;
};

void Hitbox::odcinek::prosta(Hitbox::odcinek o, float* A, float* B, float* C)
{
	*A = o.a.y - o.b.y; *B = o.b.x - o.a.x; *C = o.a.x*o.b.y - o.b.x*o.a.y;
}

bool Hitbox::odcinek::isCross(Hitbox::odcinek o1)
{
	float A1, B1, C1, A2, B2, C2;
	prosta(o1, &A1, &B1, &C1);
	prosta(*this, &A2, &B2, &C2);
	return ((((A1*a.x + B1 * a.y + C1)*(A1*b.x + B1 * b.y + C1)) < 0) && (((A2*o1.a.x + B2 * o1.a.y + C2)*(A2*o1.b.x + B2 * o1.b.y + C2)) < 0));

}

Hitbox::odcinek::odcinek()
{

}

Hitbox::Hitbox(sf::ConvexShape polygona) 
	{
		float temx, temy; // x i y wzglêdem pocz¹tku uk³adu
		punktNaOkregu tem;

		odcinek temo(sf::Vector2f(0, 0), sf::Vector2f(0, 0));//temporary
		ile = polygona.getPointCount();//tego ju¿ nie bêdzie trzeba zmieniaæ bo nie przewidujê zmiany hitboxu statku po jego stworzeniu
		polygon = polygona; // wizualizacja i wzór na stworzenie hitboxu
		for (int i = 0; i < ile; i++)
		{
			temx = polygon.getPoint(i).x;
			temy = polygon.getPoint(i).y; //znajduje punkty temx i temy wzglêdem osi o œrodku Origin

			tem.r = sqrt(pow(temx, 2) + pow(temy, 2));//d³ugoœæ tego promienia

			tem.a = (atan(temy / temx) / M_PI * 180);//k¹t pomiêdzi pionowym promieniem a promieniem do punktu temx, temy

			if (temx >= 0 && temy < 0)//ustala ¿e k¹t 0 stopni jest skierowany w górê 
			{
				tem.a = 90 + tem.a;
			}
			else
				if (temx > 0 && temy >= 0)
				{
					tem.a = tem.a + 90;
				}
				else
					if (temx <= 0 && temy > 0)
					{
						tem.a = 90 + tem.a + 180;
					}
					else
					{
						tem.a = tem.a + 270;
					}
			punkty.push_back(tem);//dodaje do zbioru punktów informacje tam zawarte ju¿ nie bêd¹ zmieniane ale u³atwiaj¹ bracanie punktów wzglêdem okna
			odcinki.push_back(temo);//rezerwuje miejsce na odcinki które trzeba bêdzie obliczaæ do obrót na postawie zbioru 'punky' i k¹tu 'a'
		}

		a = (polygon.getRotation() - 360)*-1;//zabezpieszczenie je¿eli polygon ju¿ jest obrócony 


		RotationPoint = polygon.getPosition();// je¿eli polygon ju¿ jest gdzieœ przesuniêty

		rotate(0);//uzupe³nia pierwszymi odcinkami w razie potrzeby wykrycia styku bez uprzedniego obrotu
}

void Hitbox::hitboxmove(float xs, float ys)
{
	polygon.move(sf::Vector2f(xs, ys));
	RotationPoint += sf::Vector2f(xs, ys);
	rotate(0);
}

float Hitbox::angleRotate(float oa, float r)//sprawdza czy k¹t nie przekrêci³ 360 i sprowadza go na ziemiê 
{
	oa = oa + r;
	if (oa > 360)
	{
		oa = oa - 360;
	}
	else
		if (oa < 0)
		{
			oa = oa + 360;
		}
	return oa;
}

float Hitbox::sNR(float s)
{
	return (s / 180.0) * M_PI;
}

void Hitbox::rotate(float at)//liczy nowe odcinki po obrocie na podstawie k¹tu 'a' zakualizowanego o 'at' oraz zbioru niezmiennego 'punkty' 
{
	a = angleRotate(a, at);
	polygon.setRotation(a);

	odcinki[0] = odcinek(sf::Vector2f(punkty[ile - 1].r*sin(sNR(punkty[ile - 1].a + a)) + RotationPoint.x, punkty[ile - 1].r*cos(sNR(punkty[ile - 1].a + a))*-1 + RotationPoint.y), sf::Vector2f(punkty[0].r*sin(sNR(punkty[0].a + a)) + RotationPoint.x, punkty[0].r*cos(sNR(punkty[0].a + a))*-1 + RotationPoint.y));


	for (int i = 1; i < ile; i++)
	{

		odcinki[i] = odcinek(sf::Vector2f(odcinki[i - 1].b.x, odcinki[i - 1].b.y), sf::Vector2f(punkty[i].r*sin(sNR(punkty[i].a + a)) + RotationPoint.x, punkty[i].r*cos(sNR(punkty[i].a + a))*-1 + RotationPoint.y));//r*cosa = y, r*sina = x

	}
}

void Hitbox::draw(sf::RenderWindow *window)
{
	window->draw(polygon);
}

bool Hitbox::hit(odcinek odc)
{
	for (int i = 0; i < ile; i++)
	{
		if (odc.isCross(odcinki[i]))
		{

			return 1;
		}
	}
	return 0;
}

bool Hitbox::objectHit(Hitbox obj)
{
	
	for (int i = 0; i < ile; i++)
	{
		if (obj.hit(this->odcinki[i]))
		{
			return 1;
		}
	}
	return 0;
}

Hitbox::odcinek Hitbox::getOdcinek(int i)
{
	return odcinki[i];
}

sf::Vector2f Hitbox::getPoint(int i)
{
	return odcinki[i].b;
}

sf::Vector2f Hitbox::getPosition()
{
	return polygon.getPosition();
}

void Hitbox::setColor(sf::Color c)
{
	polygon.setFillColor(c);
}