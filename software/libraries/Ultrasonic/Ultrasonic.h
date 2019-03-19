#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class Ultrasonic {
    private:
        // Pins
        const int trigPin_;
        const int echoPin_;
    public:
        // Constructor
        Ultrasonic(int trigPin, int echoPin);
        // Methods
        long ReadDistance();
        long ReadAverageDistance(int numSamples);
};

#endif
