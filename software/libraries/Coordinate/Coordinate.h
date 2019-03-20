#ifndef COORDINATE_H
#define COORDINATE_H

const int OFF_THE_MAP = -100;

class Coordinate {
    public:
        Coordinate();
        Coordinate(int _x, int _y);
        int x;
        int y;
        bool operator==(Coordinate rhs);
};

Coordinate::Coordinate() : x(OFF_THE_MAP), y(OFF_THE_MAP) {}
Coordinate::Coordinate(int _x, int _y) : x(_x), y(_y) {}

bool Coordinate::operator==(Coordinate rhs) {
    return (x == rhs.x && y == rhs.y);
}

#endif
