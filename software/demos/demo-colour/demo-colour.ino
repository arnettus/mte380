#include "Colour.h"

Colour colour(4, 7, 8, 11, 13,10, Colour::Frequency::PERCENT_20);
void setup()
{
  Serial.begin(9600);
  colour.InitColour();
 
}

unsigned long frequency;

void loop() { 
    colour.ReadColour();    
}
