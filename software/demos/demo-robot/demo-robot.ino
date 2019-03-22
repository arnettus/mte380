#include <Robot.h>
#include <SoftwareSerial.h>

Robot r;

void setup() {
    Serial.begin(9600);
}

void loop() {
    r.go();

    while(true){}
}
