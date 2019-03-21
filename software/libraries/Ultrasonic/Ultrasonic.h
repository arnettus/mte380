#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class Ultrasonic {
    private:
        // Pins
        const int trigPin_;
        const int echoPin_;
    public:
        // tile half width (150 mm) - chassis half width (63.5 mm) + ultrasonic depth (15 mm)
        static const int offset_mm = 71;
        static const int tileLength_mm = 300;
        static const int minDetection_mm = 0;
        static const int maxDetection_mm = 140;
        enum Object {
            OBJECT_NOT_FOUND,
            OBJECT_FOUND,
            ERROR
        };
        // Constructor
        Ultrasonic(int trigPin, int echoPin);
        // Methods
        long ReadDistance();
        long ReadAverageDistance(int numSamples);
        Object ReadNumTiles(int numSamples, int& numTiles);
};

#endif
