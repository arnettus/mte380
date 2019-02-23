#ifndef FLAME_H
#define FLAME_H

#include <stdio.h>
#include <Arduino.h>

enum FlameStatus {
    FLAME_NOT_FOUND,
    FLAME_IS_LEFT,
    FLAME_IS_RIGHT,
    FLAME_IS_CENTERED,
    FLAME_IS_EXTINGUISHABLE,
};

struct FlameLocation {
    FlameStatus status;
    int distance;
};

class Flame {
private:
    const int leftPin;
    const int rightPin;

public:
    Flame(int _leftPin, int _rightPin);

    int readValue(bool useLeft);
    int readDistance(bool useLeft);

    FlameLocation locate();
};

#endif
