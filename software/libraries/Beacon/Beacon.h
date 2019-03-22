#ifndef BEACON_H
#define BEACON_H

#include <Arduino.h>

class Beacon
{
public:
	enum Color{
		RED,
		GREEN,
		BLUE,
	};
	Beacon(int redPin, int greenPin, int bluePin);
	void SetColor(Color color);
	void InitLED();
	void Off();
private:
	int redPin_, greenPin_, bluePin_;
};

#endif 