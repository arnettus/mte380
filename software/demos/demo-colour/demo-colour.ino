#include "Colour.h"

Colour colour(4, 7, 8, 11, 13, Colour::Frequency::PERCENT_20);
void setup() {
    Serial.begin(9600);
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
}

unsigned long frequency;

void loop() {
    colour.PrintRGB();
}
