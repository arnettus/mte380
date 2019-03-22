#include <Navigator.h>

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
    //nav.demoIMULoopOrientation();
    //nav.demoIMULoopMagnetometer();
    nav.demoManualMode();
    return;
}
