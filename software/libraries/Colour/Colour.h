#ifndef COLOUR_H
#define COLOUR_H

#include <Arduino.h>

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
    Colour(int S0, int S1, int S2, int S3, int OUT, Frequency frequency = PERCENT_20);
    void PrintRGB(int delay_time = 100);
    ColourType GetColour(int r, int g, int b);
    ColourType ReadColour(int delay_time = 100);

private:
    int _S0, _S1, _S2, _S3, _OUT;
    Frequency _frequency;
};

#endif // COLOUR_H
