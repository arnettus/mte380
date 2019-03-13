#include "Flame.h"
#include <Arduino.h>

const int IN_RANGE = 1010;

Flame::Flame(int _leftPin, int _rightPin) : leftPin(_leftPin), rightPin(_rightPin) {}

bool Flame::isFlameInSight() {
    return (analogRead(leftPin) < IN_RANGE) || (analogRead(rightPin) < IN_RANGE);
}
