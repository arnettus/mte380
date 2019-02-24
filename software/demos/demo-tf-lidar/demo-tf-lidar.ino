#include <TFLidar.h>

TFLidar lidar(8);

void setup() {
    Serial.begin(9600);
    lidar.begin(&Serial1);

    OCR0A = 0x3F;   // approximately 4x a ms
    TIMSK0 |= _BV(OCIE0A);
}

ISR(TIMER0_COMPA_vect) {
    TFStatus st = lidar.measure();

    if (st == TF_STATUS_OKAY) {
        lidar.printLatest(&Serial, true);
    } else {
        Serial.print("Error code: ");
        Serial.println(st);
    }
}

void loop() {

}
