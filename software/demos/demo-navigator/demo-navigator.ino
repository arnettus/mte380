#include <Navigator.h>

#define USE_SERIAL_INSTEAD_OF_MOTORS false

Navigator nav;

void setup() {
    Serial.begin(9600);
    if (!nav.begin()) {
        Serial.println("Navigator failed to begin");
        while (1) {
            delay(1000);
        }
    }
}

void loop() {
    //nav.imuLoop();
    nav.manualMode(USE_SERIAL_INSTEAD_OF_MOTORS);
    return;
}
