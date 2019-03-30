#include "Flame.h"
#include <Arduino.h>

const int IN_RANGE = 970;

Flame::Flame(int _pin) : pin(_pin) {}

int Flame::readFlame() {
    return analogRead(pin);
}

int Flame::readFlameAverage(int numSamples) {
    long sum = 0;
    for(int i = 0; i < numSamples; i++){
        sum += readFlame();
    }

    return sum / numSamples;
}

bool Flame::isFlameInSight(bool useAverage, int numSamples) {
    if(useAverage) return readFlameAverage(numSamples) <= IN_RANGE;
    return readFlame() <= IN_RANGE;
}
