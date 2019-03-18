#include <Flame.h>
#include <SoftwareSerial.h>
#include <Motors.h>

// constructors
Flame leftFlame(A0);
Flame rightFlame(A1);
Motors myMotors;

// flags
bool leftFlameDetected = false;
bool rightFlameDetected = false;
volatile bool checkForFlame = false;

void setup() {
    Serial.begin(9600);

    // !!! pretty sure we don't need interrupts for this
    // noInterrupts();
    // OCR1A = 0xFFF;   // approximately every 16ms

    // TCCR1A = 0;
    // TCCR1B = (1 << WGM12)|(1 << CS11)|(1 << CS10);
    // TCNT1 = 0;

    // TIMSK1 |= _BV(OCIE1A);
    // interrupts();
}



// ISR(TIMER1_COMPA_vect) {
//     if (!pollMe)
//         pollMe = true;
// }

void loop() {
   if (checkForFlame) {
        int threshhold = 1000;
        if (leftFlame.readFlame() <= threshhold){
            // flame detected on left!
            myMotors.Halt();
            //myMotors.Rotate(-90)
            //turn the fan on
            //delay(1000)
            //turn the fan off
            //myMotors.Rotate(90)
            checkForFlame = false;
        }
        else if(rightFlame.readFlame() <= threshhold){
            // flame detected on right!
            myMotors.Halt();
            //myMotors.Rotate(90)
            //turn the fan on
            //delay(1000)
            //turn the fan off
            //myMotors.Rotate(-90)
            checkForFlame = false;
        }
        // Indicate the fire has been extinguished
        digitalWrite(13, HIGH);   
    }
}
