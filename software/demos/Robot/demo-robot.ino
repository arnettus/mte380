#include <Robot.h>

int lidarCapacity = 8;
int leftFlamePin = A0;
int rightFlamePin = A1;
int fanPin = A2;

Robot r(
    lidarCapacity,
    leftFlamePin,
    rightFlamePin,
    fanPin
);

void setup() {
    Serial.begin(9600);

    r.initializeSensors();
}

void loop() {
    r.go();
    return 0;
}
