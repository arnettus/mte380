#include <SoftwareSerial.h>
#include <Navigator.h>
#include <Colour.h>
#include <Gravity.h>
#include <Wire.h>

// Config
GravityConfig config1 = {.capture = GravityConfig::NONE,
                         .range = GravityConfig::STANDARD,
                         .address = 0x29,
                         .reset_pin = 4,
                         .timeout = 500,
                         .capacity = 10,
                         .min_range = 60,
                         .max_range = 400
};

// constructors
Navigator nav;
Colour col(4, 7, 8, 11, 13, 15);
Gravity gravity(config1);

// Flags
bool checkForColour = true;




void setup() {
    col.InitColour();
    gravity.Init();
    Serial.begin(9600);
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

// Travel up to house
nav.goForward(30);
delay(1000);
nav.turnLeft();
nav.goForward(30);
nav.turnRight();
nav.goForward(30);
delay(1000);
nav.turnLeft();
nav.goForward(30);
delay(1000);

// Approach and determine which house it is
nav.goForward(ceil(gravity.getDistance() - 50)/10);
if(col.ReadColour() == Colour::RED){
    digitalWrite(51, LOW);
    digitalWrite(52, HIGH);
    digitalWrite(53, HIGH);
} else if (col.ReadColour() == Colour::YELLOW){
    digitalWrite(51, LOW);
    digitalWrite(52, HIGH);
    digitalWrite(53, HIGH);
}





void loop() {
}
