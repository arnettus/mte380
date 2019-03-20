#include <Flame.h>
#include <SoftwareSerial.h>
#include <Navigator.h>
#include <Fan.h>

// constructors
Flame leftFlame(A0);
Flame rightFlame(A1);
Navigator nav;
Fan myFan;

// flags
bool checkForFlame = true;

void setup() {
    myFan.Setup();
    Serial.begin(9600);
        if (!nav.begin()) {
        Serial.println("Navigator failed to begin");
        while (1) {
            delay(1000);
        }
    }
    
    Serial.println("Starting in 3 seconds...");
    delay(3000);
    Serial.println("Starting!");
//    nav.goForward();
}

void loop() {
   if (checkForFlame) {
    while (true) {
        Serial.println("");
        Serial.print("Left:");
        Serial.print(leftFlame.readFlame());
        Serial.print("Right:");
        Serial.print(rightFlame.readFlame());
        Serial.println("");
        delay(500);
    }
        
        
        int threshhold = 1000;
        if (leftFlame.isFlameInSight()){
            // flame detected on left!
            Serial.print("Flame detected at: ");
            Serial.print(leftFlame.isFlameInSight());
            nav.halt();
            delay(1500);
            nav.turnLeft();
            myFan.TurnOn(Fan::LOW_SPEED);
            delay(3000);
            myFan.TurnOff();
            myFan.Shutdown();
            nav.turnRight();
        }
        else if(rightFlame.isFlameInSight()){
            // flame detected on right!
            Serial.print("Flame detected at: ");
            Serial.print(rightFlame.isFlameInSight());
            nav.halt();
            delay(1500);
            nav.turnRight();
            myFan.TurnOn();
            delay(3000);
            myFan.TurnOff();
            myFan.Shutdown();
            nav.turnLeft();
        } 
    }
}
