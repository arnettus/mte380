#include <Robot.h>
#include <SoftwareSerial.h>

Robot r(
    A0, A1,
    25, 24,
    27, 26,
    37, 35, 33, 31, 39, 15,
    51, 52, 53
);

void setup() {
    Serial.begin(9600);
    r.initializeSensors();
}

void loop() {
    delay(5000);
    
    r.go();

    return 0;
}
