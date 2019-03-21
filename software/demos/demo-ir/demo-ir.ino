#include <SoftwareSerial.h>
#include "IR.h"

Firewall firewall;

void setup() {
    Serial.begin(9600);
}

void loop() {
    // for (int i = 0; i < 4; i++) {
    //     Serial.print(firewall.ReadValue(i));
    //     Serial.print(", ");
    // }
    // Serial.println("");
    Serial.println(firewall.GetFlameLocation());
}
