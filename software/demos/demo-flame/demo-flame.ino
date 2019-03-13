#include <Flame.h>
#include <SoftwareSerial.h>

Flame flame(A0, A1);

void setup() {
    Serial.begin(9600);
}

void demoReadValue() {
    int l = flame.readValue(true);
    int r = flame.readValue(false);
    
    Serial.print("Left: ");
    Serial.print(l);
    Serial.print(", Right: ");
    Serial.print(r);
    Serial.print(", diff: ");
    Serial.println(abs(l-r));
}

void demoLocate() {
    FlameStatus fs = flame.locate();

    switch (fs) {
        case FLAME_NOT_FOUND:
            Serial.print("Flame not found\n");
            break;
        case FLAME_IS_LEFT:
            Serial.print("Flame left\n");
            break;
        case FLAME_IS_RIGHT:
            Serial.print("Flame right\n");
            break;
        case FLAME_IS_CENTERED:
            Serial.print("Flame is centered\n");
            break;
        default:
            break;
    }
}


void loop() {
      delay(25);
      demoReadValue();
      //demoLocate();
}
