#ifndef COORDINATE_H
#define COORDINATE_H

class Coordinate {
    public:
        Coordinate(int _x, int _y);
        int x;
        int y;

        bool operator==(Coordinate rhs);
};

Coordinate::Coordinate(int _x, int _y) : x(_x), y(_y) {}

bool Coordinate::operator==(Coordinate rhs) {
    return (x == rhs.x && y == rhs.y);
}

#endif
