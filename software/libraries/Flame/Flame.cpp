#include "Flame.h"

const int LEFT_DIST_MIN = 10;
const int LEFT_DIST_MAX = 300;

const int RIGHT_DIST_MIN = 10;
const int RIGHT_DIST_MAX = 300;

const int CENTER_TOL = 5;
const int EXTINGUISH_RANGE = 100;

Flame::Flame(int _leftPin, int _rightPin) : leftPin(_leftPin), rightPin(_rightPin) {}

int Flame::readValue(bool useLeft) {
    return useLeft ? analogRead(leftPin) : analogRead(rightPin);
}

int Flame::readDistance(bool useLeft) {
    return useLeft ? map(analogRead(leftPin), 0, 1023, LEFT_DIST_MIN, LEFT_DIST_MAX) : map(analogRead(rightPin), 0, 1023, RIGHT_DIST_MIN, RIGHT_DIST_MAX);
}

FlameLocation Flame::locate() {
    FlameLocation fl;

    int l = readDistance(true);
    int r = readDistance(false);
    int diff = l - r;

    fl.distance = (l + r) / 2;

    if (l == LEFT_DIST_MAX && r == RIGHT_DIST_MAX) {
        fl.status = FLAME_NOT_FOUND;
    } else if (abs(diff) < CENTER_TOL) {
        if (fl.distance <= EXTINGUISH_RANGE) {
            fl.status = FLAME_IS_EXTINGUISHABLE;
        } else
            fl.status = FLAME_IS_CENTERED;
    } else if (diff < 0) {
        fl.status = FLAME_IS_LEFT;
    } else
        fl.status = FLAME_IS_RIGHT;

    return fl;
}
