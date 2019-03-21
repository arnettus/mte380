#include <Robot.h>
#include <SoftwareSerial.h>

Robot r(
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1
);

void setup() {
    Serial.begin(9600);
}

void printCoordinates(StackArray<Coordinate> goals) {
    while(!goals.isEmpty()) {
        Coordinate g = goals.pop();

        Serial.println("");
        Serial.print("[");
        Serial.print(g.x);
        Serial.print(",");
        Serial.print(g.y);
        Serial.print("]");
        Serial.println("");
    }

}

void loop() {
    Coordinate e{5,0};
    StackArray<Coordinate> goals = r.pathPlan(e);
    printCoordinates(goals);
    while(true){}
}
