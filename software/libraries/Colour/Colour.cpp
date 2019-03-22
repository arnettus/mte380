#include "Colour.h"



Colour::Colour(int S0, int S1, int S2, int S3, int OUT, int capacity)
    : _S0(S0), _S1(S1), _S2(S2), _S3(S3), _OUT(OUT), raR_(capacity)
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

    digitalWrite(_S0, LOW);
    digitalWrite(_S1, HIGH);
}



Colour::ColourType Colour::ReadColour()
{
    digitalWrite(_S0, LOW);
    digitalWrite(_S1, HIGH);
    delay(10);

    for(int i=0; i<30; i++)
    {
        // Setting Red filtered photodiodes to be read
        digitalWrite(_S2, LOW);
        digitalWrite(_S3, LOW);
        delay(5);
        
        raR_.add(pulseIn(_OUT, LOW));


        // Setting Green filtered photodiodes to be read
        digitalWrite(_S2,HIGH);
        digitalWrite(_S3,HIGH);
        delay(5);
        raG_.add(pulseIn(_OUT, LOW));
        

        // Setting Blue filtered photodiodes to be read
        digitalWrite(_S2,LOW);
        digitalWrite(_S3,HIGH);
        delay(5); 
        raB_.add(pulseIn(_OUT, LOW));
    }

    if(raB_.read() > raG_.read())
    {
        return ColourType::YELLOW;
    }
    else if((raG_.read() - raB_.read()) > 100)
    {
        return ColourType::RED;
    }
    else
    {
        return ColourType::YELLOW;
    }
}


