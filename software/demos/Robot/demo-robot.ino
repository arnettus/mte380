#include <Robot.h>

void setup() {
    Serial.begin(9600);
    Robot r;
}

void loop() {
    r.go();
    return 0;
}
