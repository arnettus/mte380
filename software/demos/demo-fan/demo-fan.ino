#include "Fan.h"

Fan fan;

void setup() {
    fan.Setup();
}

void loop() {
    // Fan should turn on
    fan.TurnOn();
    delay(1000);
    // Fan should turn off
    fan.TurnOff();
    delay(1000);
    // Fan should be shutdown
    fan.Shutdown();
    // Fan should NOT turn on
    fan.TurnOn();
    while (true) {
    }
}
