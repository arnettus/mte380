#ifndef FLAME_H
#define FLAME_H

#include <stdio.h>

class Flame {
private:
    const int pin;

public:
    Flame(int pin);

    int readFlame();
    bool isFlameInSight();
};

#endif
