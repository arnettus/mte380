#include <SoftwareSerial.h>
#include <Motors.h>
#include <Fan.h>
#include <Flame.h>
#include <Navigator.h>

// constructors
Flame leftFlame(A0);
Flame rightFlame(A1);
Navigator nav;
Fan myFan;

// flags
bool checkForFlame = true;
bool doRotation = false;

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
}


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
            nav.turnRight();
            checkForFlame = false;
            doRotation = true;
            delay(5000);
        }
        else if(rightFlame.readFlame() <= threshhold){
            // flame detected on right!
            // flame detected on right!
            nav.halt();
            delay(1500);
            nav.turnRight();
            myFan.TurnOn(Fan::MED_SPEED);
            delay(3000);
            myFan.TurnOff();
            myFan.Shutdown();
            nav.turnLeft();
            checkForFlame = false;
            doRotation = true;
            delay(5000);
        } 
    }
    else if (doRotation){
            nav.goForward(30);
            nav.turnLeft();
            nav.turnRight();
            nav.goReverse(30);
            doRotation = false;
    }
}
