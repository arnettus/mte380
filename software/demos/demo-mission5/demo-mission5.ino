#include <Navigator.h>
#include <Beacon.h>
#include <SoftwareSerial.h>
#include <Colour.h>
#include <Gravity.h>
#include <Wire.h>
#include <Fan.h>
#include <Flame.h>

// Config
GravityConfig config1 = {.capture = GravityConfig::NONE,
                         .range = GravityConfig::STANDARD,
                         .address = 0x29,
                         .reset_pin = 40,
                         .timeout = 500,
                         .capacity = 10
};

// objects
Flame leftFlame(A0);
Flame rightFlame(A1);
Navigator nav;
Beacon    b(51, 52, 53);
Fan myFan;
Colour col(37, 35, 33, 31, 39, 15);
Gravity gravity(config1);

// flags
bool oneShot = true;
int16_t buildingDist = 0;

// magnet check
void checkForMagnet() {
    if (nav.detectedMagnet()) {
        b.SetColor(Beacon::RED);
        delay(2000);
        b.Off();
    }
}

// flame check
bool checkForFlame() {
    int threshhold = 970;
    if (leftFlame.readFlame() <= threshhold){
        // flame detected on left!
        nav.turnLeft();
        myFan.TurnOn(Fan::MED_SPEED);
        delay(2500);
        myFan.TurnOff();
        myFan.Shutdown();
        nav.turnRight();
    }
    else if(rightFlame.readFlame() <= threshhold){
        // flame detected on right!
        nav.turnRight();
        myFan.TurnOn(Fan::MED_SPEED);
        delay(2500);
        myFan.TurnOff();
        myFan.Shutdown();
        nav.turnLeft();
    } 
}

void setup() {
    col.InitColour();
    Wire.begin();
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

void loop() {
    if(oneShot){
        // ==============================ScanMode===============================
        nav.goForward(30); // 0,1
        delay(1500);
        nav.turnLeft();
        delay(500);
        nav.goForward(30); // -1, 1
        delay(500);
        nav.turnRight();
        delay(500);
        nav.goForward(30); // -1, 2
        delay(1500);
        checkForFlame();
        nav.goForward(30); // -1, 3
        delay(1500);
        checkForFlame();
        nav.goForward(30); // -1, 4
        delay(1500);
        checkForFlame();
        nav.goForward(30); // -1, 5
        delay(1500);
        checkForFlame();
        nav.turnRight();
        nav.turnRight();

        // ===============================FoodSearch============================
        nav.goForward(30);
        delay(500);
        nav.turnLeft();
        delay(500);
        nav.goForward(62);
        delay(500);
        checkForMagnet();

        // ===============================PeopleSearch==========================
        nav.goReverse(32);
        delay(500);
        nav.turnRight();
        delay(500);
        nav.goForward(30);
        delay(500);
        nav.turnLeft();
        delay(500);
        nav.goForward(30);
        delay(500);
        buildingDist = ceil(gravity.GetDistance() - 50)/10;
        nav.goForward(buildingDist);
        if(col.ReadColour() == Colour::RED){
            digitalWrite(51, LOW);
            digitalWrite(52, HIGH);
            digitalWrite(53, HIGH);
        } else if (col.ReadColour() == Colour::YELLOW){
            digitalWrite(51, LOW);
            digitalWrite(52, HIGH);
            digitalWrite(53, HIGH);
        } else {
            digitalWrite(51, HIGH);
            digitalWrite(52, HIGH);
            digitalWrite(53, HIGH);
        } // yellow house done==================================================
        delay(2000);
        digitalWrite(51, HIGH);
        digitalWrite(52, HIGH);
        digitalWrite(53, HIGH);
        nav.goReverse(buildingDist);
        delay(500);
        nav.turnRight();
        delay(500);
        nav.turnRight();
        delay(500);
        nav.goForward(30);
        delay(500);
        nav.goForward(30);
        delay(500);
        nav.goForward(30);
        delay(500);
        nav.turnLeft();
        delay(500);
        nav.goForward(30);
        delay(500);
        nav.turnRight();
        buildingDist = ceil(gravity.GetDistance() - 50)/10;
        nav.goForward(buildingDist);
        if(col.ReadColour() == Colour::RED){
            digitalWrite(51, LOW);
            digitalWrite(52, HIGH);
            digitalWrite(53, HIGH);
        } else if (col.ReadColour() == Colour::YELLOW){
            digitalWrite(51, LOW);
            digitalWrite(52, HIGH);
            digitalWrite(53, HIGH);
        } else {
            digitalWrite(51, HIGH);
            digitalWrite(52, HIGH);
            digitalWrite(53, HIGH);
        } // red house done ====================================================
        delay(2000);
        digitalWrite(51, HIGH);
        digitalWrite(52, HIGH);
        digitalWrite(53, HIGH);

        // =================================GoHome==============================
        nav.goReverse(buildingDist);
        delay(500);
        nav.turnLeft();
        delay(500);
        nav.turnLeft();
        delay(500);
        nav.goForward(30);
        delay(500);
        nav.turnRight();
        delay(500);
        nav.goForward(30);
        delay(500);
        nav.turnLeft();
        delay(500);
        nav.goForward(30);
        delay(500);
        nav.turnRight();
        delay(500);
        nav.goForward(30);
    } 
}