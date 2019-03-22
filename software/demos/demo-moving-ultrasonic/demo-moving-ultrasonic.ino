#include <Navigator.h>
#include <Ultrasonic.h>

Navigator n;
int leftTrigPin = 25;
int leftEchoPin = 24;
int rightTrigPin = 27;
int rightEchoPin = 26;
long duration, cm;

// 24 left echo 25 left trigger
// 26 right echo 27 right trigger

Ultrasonic left_us(leftTrigPin, leftEchoPin);
Ultrasonic right_us(rightTrigPin, rightEchoPin);

int num_tiles;
int max_left_tiles = 5;
int max_right_tiles = 5;
int num_reads = 100;

void PrintUltrasonic() {
    Serial.print("Left: ");
    Serial.print(left_us.ReadNumTiles(num_reads, max_left_tiles, num_tiles));
    Serial.print(" status, ");
    Serial.print(num_tiles);
    Serial.println(" tiles");

    Serial.print("Right: ");
    Serial.print(right_us.ReadNumTiles(num_reads, max_right_tiles, num_tiles));
    Serial.print(" status, ");
    Serial.print(num_tiles);
    Serial.println(" tiles");

    delay(16);
}

void setup() {
    Serial.begin(9600);
    n.begin();

    delay(5000);
    Serial.println("spot 1");
    PrintUltrasonic();
    n.goForward(30);
    delay(1000);
    Serial.println("spot 2");
    PrintUltrasonic();
    n.goForward(30);
    delay(1000);
    Serial.println("spot 3");
    PrintUltrasonic();
    n.turnLeft();
    delay(1000);
    n.goForward(30);
    delay(1000);
    n.turnRight();
    delay(1000);
    Serial.println("spot 4");
    PrintUltrasonic();
    n.goForward(30);
    delay(1000);
    Serial.println("spot 5");
    PrintUltrasonic();
    n.goForward(30);
    delay(1000);
    Serial.println("spot 6");
    PrintUltrasonic();
    n.goForward(30);
    delay(1000);
    Serial.println("spot 7");
    PrintUltrasonic();
}

void loop() {
  
}
