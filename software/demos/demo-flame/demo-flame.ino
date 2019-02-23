#include <Flame.h>
#include <SoftwareSerial.h>

Flame flame(A0, A1);

void setup() {
    Serial.begin(9600);

    OCR0A = 0x3F;   // approximately 4x a ms
    TIMSK0 |= _BV(OCIE0A);
}

void demoReadValue(bool useLeft) {
    Serial.print(flame.readValue(useLeft));
}

void demoReadDistance(bool useLeft) {
    Serial.print(flame.readDistance(useLeft));
}

void demoLocate() {
    char msg[128];
    FlameLocation fl = flame.locate();

    switch (fl.status) {
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
            snprintf(msg, 128, "Flame is centered: %d\n", fl.distance);
            Serial.print(msg);
            break;
        case FLAME_IS_EXTINGUISHABLE:
            snprintf(msg, 128, "Flame is extinguishable: %d\n", fl.distance);
            break;
        default:
            break;
    }
}

ISR(TIMER0_COMPA_vect) {
    demoReadValue(true);
    //demoReadValue(false);
    //demoDistanceValue(true);
    //demoDistanceValue(false);
    //demoLocate();
}

void loop() {

}
