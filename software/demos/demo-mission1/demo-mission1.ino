#include <Flame.h>
#include <SoftwareSerial.h>
#include <Motors.h>
#include <Fan.h>
#include <Flame.h>

// constructors
Flame leftFlame(A0);
Flame rightFlame(A1);
Navigator nav;
Fan myFan;

// flags
bool leftFlameDetected = false;
bool rightFlameDetected = false;
volatile bool checkForFlame = false;

void setup() {
    myFan.Setup();
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
        int threshhold = 980;
        if (leftFlame.readFlame() <= threshhold){
            // flame detected on left!
            nav.halt();
            delay(1500);
            nav.turnLeft();
            myFan.TurnOn(Fan::MED_SPEED);
            delay(3000);
            myFan.TurnOff();
            myFan.Shutdown();
            myMotors.OLTurnRight90();
            checkForFlame = false;
        }
        else if(rightFlame.readFlame() <= threshhold){
            // flame detected on right!
            nav.halt();
            delay(1500);
            nav.turnRight();
            myFan.TurnOn(Fan::MED_SPEED);
            delay(3000);
            myFan.TurnOff();
            myFan.Shutdown();
            myMotors.OLTurnLeft90();
            checkForFlame = false;
        } 
    }
}
