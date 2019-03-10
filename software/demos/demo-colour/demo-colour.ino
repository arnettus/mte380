#include "Colour.h"

Colour colour(28, 26, 24, 22, 30, Colour::Frequency::PERCENT_20);
void setup() {
    Serial.begin(9600);
}

unsigned long frequency;

void loop() {
    colour.PrintRGB();
}