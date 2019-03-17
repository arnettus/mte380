#include "Colour.h"

int RED_R_LOW = 0;
int RED_R_HIGH = 50;
int RED_G_LOW = 0;
int RED_G_HIGH = 50;
int RED_B_LOW = 0;
int RED_B_HIGH = 50;

int YELLOW_R_LOW = 0;
int YELLOW_R_HIGH = 50;
int YELLOW_G_LOW = 0;
int YELLOW_G_HIGH = 50;
int YELLOW_B_LOW = 0;
int YELLOW_B_HIGH = 50;


Colour::Colour(int S0, int S1, int S2, int S3, int OUT, Frequency frequency)
    : _S0(S0), _S1(S1), _S2(S2), _S3(S3), _OUT(OUT), _frequency(frequency) {
    pinMode(_S0, OUTPUT);
    pinMode(_S1, OUTPUT);
    pinMode(_S2, OUTPUT);
    pinMode(_S3, OUTPUT);
    pinMode(_OUT, INPUT);

    digitalWrite(_S0, _frequency >> 1);
    digitalWrite(_S1, _frequency & 1);
}

void Colour::PrintRGB(int delay_time) {
    unsigned long frequency;

    // Setting Red filtered photodiodes to be read
    digitalWrite(_S0, HIGH);
    digitalWrite(_S1, LOW);
    frequency = pulseIn(_OUT, LOW);
    Serial.print("R= ");
    Serial.print(frequency);
    Serial.print("  ");
    delay(delay_time);

    // Setting Green filtered photodiodes to be read
    digitalWrite(_S2,HIGH);
    digitalWrite(_S3,HIGH);
    frequency = pulseIn(_OUT, LOW);
    // Printing the value on the serial monitor
    Serial.print("G= ");
    Serial.print(frequency);
    Serial.print("  ");
    delay(delay_time);

    // Setting Blue filtered photodiodes to be read
    digitalWrite(_S2,LOW);
    digitalWrite(_S3,HIGH);
    frequency = pulseIn(_OUT, LOW);
    // Printing the value on the serial monitor
    Serial.print("B= ");
    Serial.print(frequency);
    Serial.println("  ");
    delay(delay_time);
}

Colour::ColourType Colour::ReadColour(int delay_time) {
    unsigned long red_freq, green_freq, blue_freq;

    // Setting Red filtered photodiodes to be read
    digitalWrite(_S0, HIGH);
    digitalWrite(_S1, LOW);
    red_freq = pulseIn(_OUT, LOW);
    delay(delay_time);

    // Setting Green filtered photodiodes to be read
    digitalWrite(_S2,HIGH);
    digitalWrite(_S3,HIGH);
    green_freq = pulseIn(_OUT, LOW);
    delay(delay_time);

    // Setting Blue filtered photodiodes to be read
    digitalWrite(_S2,LOW);
    digitalWrite(_S3,HIGH);
    blue_freq = pulseIn(_OUT, LOW);
    delay(delay_time);

    return GetColour(red_freq, green_freq, blue_freq);
}

Colour::ColourType Colour::GetColour(int r, int g, int b) {
    if (g > r && g > b) {
        return ColourType::RED;
    }

    return ColourType::YELLOW;
}
