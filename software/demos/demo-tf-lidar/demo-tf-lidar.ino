#include <TFLidar.h>

TFLidar lidar(8);

void setup() {
    Serial.begin(9600);
    lidar.begin(&Serial1);

    OCR0A = 0x3F;   // approximately 4x a ms
    TIMSK0 |= _BV(OCIE0A);
}

volatile bool pollMe = false;

ISR(TIMER0_COMPA_vect) {
    if (!pollMe)
        pollMe = true;
}

void loop() {
    if (pollMe) {
        pollMe = false;
        TFStatus st = lidar.measure();

        if (st == TF_STATUS_OKAY) {
            lidar.printLatest(&Serial, true);
        } else if (st != TF_STATUS_ERROR_BAD_CHECKSUM) {
            Serial.print("Error code: ");
            Serial.println(st);
        }       
    }

}
