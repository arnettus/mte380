#include <Ultrasonic.h>

//int trigPin = 25;
//int echoPin = 24;
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

void PrintUltrasonic() {
    Serial.print("Left: ");
    Serial.print(left_us.ReadNumTiles(250, max_left_tiles, num_tiles));
    Serial.print(" status, ");
    Serial.print(num_tiles);
    Serial.println(" tiles");

    Serial.print("Right: ");
    Serial.print(right_us.ReadNumTiles(250, max_right_tiles, num_tiles));
    Serial.print(" status, ");
    Serial.print(num_tiles);
    Serial.println(" tiles");

    delay(16);
}

void setup() {
  //Serial Port begin
  Serial.begin (9600);
  Serial.print("Starting...");
}

void loop() {
  PrintUltrasonic();

//  delay(16);
}
