#include <Ultrasonic.h>

//int trigPin = 25;
//int echoPin = 24;
int trigPin = 27;
int echoPin = 26;
long duration, cm;

// 24 left echo 25 left trigger
// 26 right echo 27 right trigger

Ultrasonic US(trigPin, echoPin);

void setup() {
  //Serial Port begin
  Serial.begin (9600);
  Serial.print("Starting...");
}

int num_tiles;
int max_tiles = 1;

void loop() {
  Serial.print(US.ReadNumTiles(1000, max_tiles, num_tiles));
  Serial.print(" status, ");
  Serial.print(num_tiles);
  Serial.print(" tiles");
//  Serial.print(US.ReadAverageDistance(1000));
  Serial.println();

  delay(16);
}
