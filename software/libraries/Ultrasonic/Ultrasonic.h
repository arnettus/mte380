#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class Ultrasonic {
private:
    const int trigPin_;
    const int echoPin_;

public:
    Ultrasonic(int trigPin, int echoPin);

    long ReadDistance();
};

#endif
