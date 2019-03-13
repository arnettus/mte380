#include "Flame.h"

const int LEFT_DIST_MAX = 1019;
const int RIGHT_DIST_MAX = 1019;
const int CENTER_TOL = 7;

Flame::Flame(int _leftPin, int _rightPin) : leftPin(_leftPin), rightPin(_rightPin) {}

int Flame::readValue(bool useLeft) {
    return useLeft ? analogRead(leftPin) : analogRead(rightPin);
}

FlameStatus Flame::locate() {
    FlameStatus fs;

    int l = readValue(true);
    int r = readValue(false);
    int diff = l - r;

    if (l >= LEFT_DIST_MAX && r >= RIGHT_DIST_MAX) {
        fs = FLAME_NOT_FOUND;
    } else if (abs(diff) < CENTER_TOL) {
    	fs = FLAME_IS_CENTERED;
    } else if (diff < 0) {
        fs = FLAME_IS_LEFT;
    } else {
        fs = FLAME_IS_RIGHT;
    }

    return fs;
}
