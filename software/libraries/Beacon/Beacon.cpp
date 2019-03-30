#include "Beacon.h"

Beacon::Beacon(int redPin, int greenPin, int bluePin)
	: redPin_(redPin)
	, greenPin_(greenPin)
	, bluePin_(bluePin)    {
}

void Beacon::SetColor(Color color)
{
	switch(color)
	{
		case RED:
			digitalWrite(redPin_, LOW);
			digitalWrite(greenPin_, HIGH);
			digitalWrite(bluePin_, HIGH);
		break;
		case GREEN:
			digitalWrite(redPin_, HIGH);
			digitalWrite(greenPin_, LOW);
			digitalWrite(bluePin_, HIGH);
		break;
		case BLUE:
			digitalWrite(redPin_, HIGH);
			digitalWrite(greenPin_, HIGH);
			digitalWrite(bluePin_, LOW);
		break;
		default:
			digitalWrite(redPin_, HIGH);
			digitalWrite(greenPin_, HIGH);
			digitalWrite(bluePin_, HIGH);
		break;

	}
}

void Beacon::InitLED()
{
	pinMode(redPin_, OUTPUT);
	pinMode(greenPin_, OUTPUT);
	pinMode(bluePin_, OUTPUT);
	digitalWrite(redPin_, HIGH);
	digitalWrite(greenPin_, HIGH);
	digitalWrite(bluePin_, HIGH);
}

void Beacon::Off()
{
	digitalWrite(redPin_, HIGH);
	digitalWrite(greenPin_, HIGH);
	digitalWrite(bluePin_, HIGH);
}
