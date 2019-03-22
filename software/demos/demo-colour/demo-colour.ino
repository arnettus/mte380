#include "Colour.h"

Colour colour(4, 7, 8, 11, 13,15);
void setup()
{
  Serial.begin(9600);
  colour.InitColour();
 
}
int counter = 0;

void loop() {   
    if(Serial.available())
    {
       counter++;
       Serial.read();
       Serial.print(counter);
       Serial.print("= ");
       if(colour.ReadColour() == Colour::RED)
       {
        Serial.println("Red House");
       }
       else
       {
        Serial.println("Yellow House");
       }
       //colour.Print2Freq();
    }
}

