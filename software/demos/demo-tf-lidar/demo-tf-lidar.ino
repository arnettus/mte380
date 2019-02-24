#include <TFLidar.h>

TFLidar lidar(8); // takes Serial1 on the Mega

void setup() {
    Serial.begin(9600);
    lidar.begin();

    OCR0A = 0x3F;   // approximately 4x a ms
    TIMSK0 |= _BV(OCIE0A);
}

ISR(TIMER0_COMPA_vect) {
    char msg[128];
    TFStatus st = lidar.measure();

    if (st == TF_STATUS_OKAY) {
        snprintf(msg, 128, "Distance: %llu, Strength: %llu\n\n", lidar.getDistanceRaw(), lidar.getStrength());
        Serial.print(msg);
    } else {
        snprintf(msg, 128, "Error code: %d\n\n", st);
        Serial.print(msg);
    }
}

void loop() {

}
