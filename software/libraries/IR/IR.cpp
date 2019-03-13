#include "IR.h"

IR::IR(int _pin) : pin(_pin) {}

int IR::readDistance() {
    float volts = analogRead(pin)*0.0048828125; // value from sensor * (5/1024)
    int distance = 13*pow(volts, -1); // worked out from datasheet graph

    return distance;
}

int IR::readValue() {
    return analogRead(pin);
}
