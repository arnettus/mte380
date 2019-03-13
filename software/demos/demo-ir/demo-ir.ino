#include <IR.h>
#include <SoftwareSerial.h>

IR ir(A0);

void setup() {
    pinMode(A0, INPUT);
    Serial.begin(9600);
    delay(100);
}

void loop() {
    int dist = ir.readDistance();
    // Serial.println(ir.readValue());
    delay(1000); // slow down serial port

    if (dist <= 30){
        Serial.println(dist);   // print the distance
    }
}
