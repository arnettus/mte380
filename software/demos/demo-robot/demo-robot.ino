#include <Robot.h>
#include <SoftwareSerial.h>

int lidarCapacity = 8;
int leftFlamePin = A0;
int rightFlamePin = A1;
int fanPin = A2;

Robot r(
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1
);

void setup() {
    Serial.begin(9600);
}

void loop() {
    r.go();
    Serial.println();
    delay(500);
    return 0;
}
