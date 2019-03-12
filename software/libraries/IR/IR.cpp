#include "IR.h"

IR::IR(int _pin) : pin(pin) {}

int IR::readDistance() {
    volts = analogRead(sensor)*0.0048828125; // value from sensor * (5/1024)
    distance = 13*pow(volts, -1); // worked out from datasheet graph

    return distance
}
