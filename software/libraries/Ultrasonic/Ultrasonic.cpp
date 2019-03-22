#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(int trigPin, int echoPin) : trigPin_(trigPin), echoPin_(echoPin) {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

long Ultrasonic::ReadDistance() {
    digitalWrite(trigPin_, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin_, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin_, LOW);

    return (pulseIn(echoPin_, HIGH)/2) / 29.1; // returns cm value
}

long Ultrasonic::ReadAverageDistance(int numSamples){
    long sum = 0;
    for(int i = 0; i < numSamples; i++){
        sum += Ultrasonic::ReadDistance();
    }
    return sum / numSamples;
}

Ultrasonic::Object Ultrasonic::ReadNumTiles(int numSamples, int maxTiles, int& numTiles) {
    int num_tiles;
    long value = ReadAverageDistance(numSamples)*10 - offset_mm;

    if (value < minDetection_mm) {
        numTiles = 0;
        return ERROR;
    } else if (value > tileLength_mm*maxTiles - maxOffset_mm) {
        numTiles = (value + tileLength_mm) / tileLength_mm;
        return OBJECT_NOT_FOUND;
    }

    numTiles = (value + tileLength_mm) / tileLength_mm;
    return OBJECT_FOUND;
}
