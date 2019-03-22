#ifndef COLOUR_H
#define COLOUR_H

#include <Arduino.h>
#include "RollingAverage.h"

class Colour {
public:
    enum ColourType {
        RED,
        YELLOW,
        UNKNOWN
    };
    enum Frequency {
        OFF = 0,
        PERCENT_2 = 0x01,
        PERCENT_20 = 0x10,
        PERCENT_100= 0x11
    };
    Colour(int S0, int S1, int S2, int S3, int OUT,  int capacity);
    void InitColour();
    ColourType ReadColour();

private:
    int _S0, _S1, _S2, _S3, _OUT;
    RollingAverage<uint16_t> raR_;
    RollingAverage<uint16_t> raG_;
    RollingAverage<uint16_t> raB_;
};

#endif // COLOUR_H
