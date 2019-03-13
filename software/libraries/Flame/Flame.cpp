#include "Flame.h"
#include <Arduino.h>

const int IN_RANGE = 1000;
const int TOL = 100;

Flame::Flame(int _leftPin, int _rightPin) : leftPin(_leftPin), rightPin(_rightPin) {}

int Flame::readRightFlame() {
    return analogRead(rightPin);
}

int Flame::readLeftFlame() {
    return analogRead(leftPin);
}

bool Flame::isFlameInSight() {
    int l = analogRead(leftPin);
    int r = analogRead(rightPin);

    if (l > IN_RANGE || r > IN_RANGE) {
        return false;
    }

    return abs(l - r) < TOL;
}

// if the difference between them is low and the
// neither of them are out of range, then light the candle
// the wirst case scenario would be when they are
