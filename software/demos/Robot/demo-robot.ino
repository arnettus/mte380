#include <Robot.h>

Robot r;

void setup() {
    Serial.begin(9600);
    r.init();
}

void loop() {
    r.go();
    return 0;
}
