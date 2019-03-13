#include <TFLidar.h>

TFLidar lidar(6);

void setup() {
    Serial.begin(9600);
    lidar.begin(&Serial3);

    noInterrupts();
    OCR1A = 0xFFF;   // approximately every 16ms

    TCCR1A = 0;
    TCCR1B = (1 << WGM12)|(1 << CS11)|(1 << CS10);
    TCNT1 = 0;

    TIMSK1 |= _BV(OCIE1A);
    delay(5000);
    interrupts();
}

volatile bool pollMe = false;

ISR(TIMER1_COMPA_vect) {
    if (!pollMe)
        pollMe = true;
}

// unsigned long oldTime = 0;

void loop() {
// interrrupts
   if (pollMe) {
        TFStatus st = lidar.measure();

        if (st == TF_STATUS_OKAY) {
            lidar.printLatest(&Serial);
        } else if (st != TF_STATUS_ERROR_BAD_CHECKSUM) {
            Serial.print("Error code: ");
            Serial.println(st);
        }
        pollMe = false;
    }

    // polling
    //   unsigned long newTime = millis();

    //   if ((newTime - oldTime) > 5) {
    //       TFStatus st = lidar.measure();
    //       if (st == TF_STATUS_OKAY) {
    //           lidar.printLatest(&Serial, true);
    //       } else if (st != TF_STATUS_ERROR_BAD_CHECKSUM) {
    //           Serial.print("Error code: ");
    //           Serial.println(st);
    //       }
    //       oldTime = newTime;
    //   }



}
