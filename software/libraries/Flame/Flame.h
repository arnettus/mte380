#ifndef FLAME_H
#define FLAME_H

#include <stdio.h>

class Flame {
private:
    const int leftPin;
    const int rightPin;

public:
    Flame(int _leftPin, int _rightPin);

    bool isFlameInSight();
};

#endif
