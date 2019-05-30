#include "pch.h"
#include "odcinek.h"

odcinek::odcinek(sf::Vector2f a, sf::Vector2f b) : punkt1(a), punkt2(b)
{
	orgin = sf::Vector2f(0, 0);

	line = sf::VertexArray(sf::LineStrip, 2);
	line[0].position = a;
	line[1].position = b;
};

sf::Vector2f punktNaOkregu::zamienNaPunkt(sf::Vector2f orgin)
{
	return sf::Vector2f(this->r*sin(sNR(this->a)), -this->r * cos(sNR(this->a))) + orgin;
}

float sNR(float s)
{
	return (s / 180.0) * M_PI;
}

void odcinek::rotate(float angle)
{
	oa = zamienNaPunktNaOkregu(punkt1, orgin);
	ob = zamienNaPunktNaOkregu(punkt2, orgin);
	
	this->oa.r = this->oa.r + angle;
	if (this->oa.r > 360)
	{
		this->oa.r -= 360;
	}
	this->ob.r = this->ob.r + angle;
	if (this->ob.r > 360)
	{
		this->ob.r -= 360;
	}


	punkt1 = oa.zamienNaPunkt(orgin);
	punkt2 = ob.zamienNaPunkt(orgin);
}
void odcinek::setRotation(float angle)
{
	oa = zamienNaPunktNaOkregu(punkt1, orgin);
	ob = zamienNaPunktNaOkregu(punkt2, orgin);

	this->oa.r = angle;
	this->ob.r = angle;
	  
	punkt1 = oa.zamienNaPunkt(orgin);
	punkt2 = ob.zamienNaPunkt(orgin);
}

void odcinek::setOrgin(sf::Vector2f norgin)
{
	orgin = norgin;
}

odcinek::odcinek()
{
}

void odcinek::updateVisual()
{
	line[0].position = punkt1;
	line[1].position = punkt2;
}


void odcinek::prosta(odcinek o, float &A, float &B, float &C)
{
	A = o.punkt1.y - o.punkt2.y; B = o.punkt2.x - o.punkt1.x; C = o.punkt1.x*o.punkt2.y - o.punkt2.x*o.punkt1.y;
}

bool odcinek::intersects(odcinek o1)
{
	float A1, B1, C1, A2, B2, C2;
	prosta(o1, A1, B1, C1);
	prosta(*this, A2, B2, C2);
	return ((((A1*punkt1.x + B1 * punkt1.y + C1)*(A1*punkt2.x + B1 * punkt2.y + C1)) < 0) && (((A2*o1.punkt1.x + B2 * o1.punkt1.y + C2)*(A2*o1.punkt2.x + B2 * o1.punkt2.y + C2)) < 0));
}

void odcinek::move(sf::Vector2f moveBy)
{
	punkt1 += moveBy;
	punkt2 += moveBy;
	orgin += moveBy;
}

punktNaOkregu zamienNaPunktNaOkregu(sf::Vector2f punkt, sf::Vector2f srodekOkregu)
{
	punktNaOkregu tem;
	tem.r = sqrt(pow(punkt.x, 2) + pow(punkt.y, 2));//d�ugo�� tego promienia

	tem.a = (atan(punkt.y / punkt.x) / M_PI * 180);//k�t pomi�dzi pionowym promieniem a promieniem do punktu temx, temy

	if (punkt.x >= 0)//ustala �e k�t 0 stopni jest skierowany w g�r� 
	{
		tem.a += 90;
	}
	else if (punkt.x < 0)
	{
		tem.a += 270;
	}
	return tem;
}