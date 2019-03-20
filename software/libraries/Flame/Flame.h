#ifndef FLAME_H
#define FLAME_H

#include <stdio.h>

class Flame {
private:
    const int pin;

public:
    Flame(int pin);

    int readFlame();
    int readFlameAverage(int numSamples = 100);
    bool isFlameInSight(bool useAverage = true, int numSamples = 100);
};

#endif
