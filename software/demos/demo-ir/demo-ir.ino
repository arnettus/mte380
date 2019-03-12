#include <Flame.h>
#include <SoftwareSerial.h>

IR ir(A0);

void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.println(ir.readDistance());
    delay(250); // slow down serial port
}
