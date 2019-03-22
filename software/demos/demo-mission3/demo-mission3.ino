#include <SoftwareSerial.h>
#include <Navigator.h>
#include <Colour.h>
#include <Gravity.h>
#include <Wire.h>

// Config
GravityConfig config1 = {.capture = GravityConfig::NONE,
                         .range = GravityConfig::STANDARD,
                         .address = 0x29,
                         .reset_pin = 40,
                         .timeout = 500,
                         .capacity = 10
};

// constructors
Navigator nav;
Colour col(37, 35, 33, 31, 39, 15);
Gravity gravity(config1);

// Flags
bool checkForColour = true;




void setup() {
    col.InitColour();
    Wire.begin();
    gravity.Init();
    Serial.begin(9600);
    Serial.println("Starting..");
        if (!nav.begin()) {
        Serial.println("Navigator failed to begin");
        while (1) {
            delay(1000);
        }
    }

    // Config pins for LED
    pinMode(51, OUTPUT);
    pinMode(52, OUTPUT);
    pinMode(53, OUTPUT);
    digitalWrite(51, HIGH);
    digitalWrite(52, HIGH);
    digitalWrite(53, HIGH);
    
    Serial.println("Starting in 3 seconds...");
    delay(3000);
    Serial.println("Starting!");
}





void loop() {
    if(checkForColour){
        // Travel up to house
        nav.goForward(30);
        delay(2000); // scan
        nav.turnLeft();
        delay(1000);
        nav.goForward(30);
        delay(1000);
        nav.turnRight();
        delay(1000);
        nav.goForward(30);
        delay(2000); // scan
        nav.goForward(30);
        delay(2000); //scan 
        nav.turnRight();
        delay(1000);
        nav.goForward(30);
        delay(1000);
        nav.goForward(30);
        delay(1000);

        // Approach and determine which house it is
        nav.goForward(ceil(gravity.GetDistance() - 50)/10);
        Colour::ColourType detectedColour = col.ReadColour();
        if(detectedColour == Colour::RED){
            digitalWrite(51, LOW);
            digitalWrite(52, HIGH);
            digitalWrite(53, HIGH);
        } else if(detectedColour == Colour::YELLOW){
            digitalWrite(51, HIGH);
            digitalWrite(52, LOW);
            digitalWrite(53, HIGH);
        }
        else {
            digitalWrite(51, HIGH);
            digitalWrite(52, HIGH);
            digitalWrite(53, HIGH);
        }
        checkForColour = false;
    }
}