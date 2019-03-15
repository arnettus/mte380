#include <Navigator.h>

Navigator nav;

void setup() {
    if (!nav.begin()) {
        Serial.println("Navigator failed to begin");
        while (1) {
            delay(1000);
        }
    }
}

void loop() {
    nav.manualMode(true);
    return;
}
