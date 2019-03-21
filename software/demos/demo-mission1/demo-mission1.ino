#include <SoftwareSerial.h>
#include <Navigator.h>
#include <Fan.h>
#include <IR.h>

// constructors
Firewall firewall;
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
    nav.goForward();
}

void loop() {
   if (checkForFlame) {
        int threshhold = 1000;
        if (firewall.ReadValue(0) <= threshhold AND firewall.ReadValue(2) <= threshhold){
            // flame detected on left!
            Serial.print("Left flame detected at: ");
            Serial.println(firewall.ReadValue(0), firewall.ReadValue(2));
            nav.halt();
            delay(1500);
            nav.turnLeft();
            myFan.TurnOn(Fan::HIGH_SPEED);
            delay(3000);
            myFan.TurnOff();
            myFan.Shutdown();
            nav.turnRight();
        }
        else if(firewall.ReadValue(1) <= threshhold AND firewall.ReadValue(3) <= threshhold){
            // flame detected on right!
            Serial.print("Right flame detected at: ");
            Serial.println(firewall.ReadValue(1), firewall.ReadValue(3));
            nav.halt();
            delay(1500);
            nav.turnRight();
            myFan.TurnOn(FAN::HIGH_SPEED);
            delay(3000);
            myFan.TurnOff();
            myFan.Shutdown();
            nav.turnLeft();
        } 
    }
}
