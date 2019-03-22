#include <Navigator.h>
#include <Beacon.h>

Navigator n;
Beacon    b(51, 52, 53);

void checkForMagnet() {
    n.readMagnetometer();
    if (n.detectedMagnet()) {
        Serial.println("OKAY");
        b.SetColor(Beacon::RED);
        delay(2000);
        b.Off();
    } else {

      Serial.println("NOT OKAY");
    }
}

void setup() {
    Serial.begin(9600);
    b.InitLED();
    b.Off();
    n.begin();

    delay(5000);

    n.goForward(30);
    n.turnLeft();
    n.goForward(60);
    Serial.println("Check pit one");
    delay(3000);
    checkForMagnet();
    n.goReverse(60);
    n.turnRight();
    n.goForward(30);
    Serial.println("Check pit two");
    delay(3000);
    checkForMagnet();
    n.goForward(60);
    n.turnRight();
    n.goForward(30);
    Serial.println("Check pit three");
    delay(3000);
    checkForMagnet();
}

void loop() {

}
