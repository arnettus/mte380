#include "Colour.h"



Colour::Colour(int S0, int S1, int S2, int S3, int OUT, int capacity, Frequency frequency)
    : _S0(S0), _S1(S1), _S2(S2), _S3(S3), _OUT(OUT), _frequency(frequency), raR_(capacity)
    , raG_(capacity), raB_(capacity) 
{
    
}

void Colour::InitColour()
{
    pinMode(_S0, OUTPUT);
    pinMode(_S1, OUTPUT);
    pinMode(_S2, OUTPUT);
    pinMode(_S3, OUTPUT);
    pinMode(_OUT, INPUT);

    digitalWrite(_S0, _frequency >> 1);
    digitalWrite(_S1, _frequency & 1);
}

Colour::ColourType Colour::ReadColour(int delay_time)
{
    digitalWrite(_S0, _frequency >> 1);
    digitalWrite(_S1, _frequency & 1);

    unsigned long frequency;
    for(int i=0; i<30; i++)
    {
        // Setting Red filtered photodiodes to be read
        digitalWrite(_S2, LOW);
        digitalWrite(_S3, LOW);
        frequency = pulseIn(_OUT, LOW);
        raR_.add(frequency);


        // Setting Green filtered photodiodes to be read
        digitalWrite(_S2,HIGH);
        digitalWrite(_S3,HIGH);
        frequency = pulseIn(_OUT, LOW);
        raG_.add(frequency);
        

        // Setting Blue filtered photodiodes to be read
        digitalWrite(_S2,LOW);
        digitalWrite(_S3,HIGH);
        frequency = pulseIn(_OUT, LOW);
        raB_.add(frequency);
    }
    if(raB_.read() > raG_.read())
    {
        Serial.println("BOSTON BRUINS");
        return ColourType::YELLOW;
    }
    else if((raG_.read() - raB_.read()) > 8)
    {
        Serial.println("MONTREAL CANADIANS");
        return ColourType::RED;
    }
    else
    {
        Serial.println("BOSTON BRUINS");
        return ColourType::YELLOW;
    }
    delay(1000);
}


