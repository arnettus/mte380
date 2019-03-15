#include <Robot.h>
#include <Arduino.h> // does this have too much?

void setup() {
    Serial.begin(9600);

    // pass in sensor pins?

    Robot r;
}

void loop() {
    r.go();

    return 0;
}
