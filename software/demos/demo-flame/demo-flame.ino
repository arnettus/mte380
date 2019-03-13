#include <Flame.h>
#include <SoftwareSerial.h>

Flame f(A0, A1);

void setup() {
    Serial.begin(9600);

    noInterrupts();
    OCR1A = 0xFFF;   // approximately every 16ms

    TCCR1A = 0;
    TCCR1B = (1 << WGM12)|(1 << CS11)|(1 << CS10);
    TCNT1 = 0;

    TIMSK1 |= _BV(OCIE1A);
    interrupts();
}

volatile bool pollMe = false;

ISR(TIMER1_COMPA_vect) {
    if (!pollMe)
        pollMe = true;
}

void loop() {
   if (pollMe) {
        Serial.println(f.isFlameInSight());
        pollMe = false;
    }
}
