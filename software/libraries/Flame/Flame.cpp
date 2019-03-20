#include "Flame.h"
#include <Arduino.h>

const int IN_RANGE = 1000;

Flame::Flame(int _pin) : pin(_pin) {}

int Flame::readFlame() {
    return analogRead(pin);
}

bool Flame::isFlameInSight() {
    return analogRead(pin) <= IN_RANGE;
}
