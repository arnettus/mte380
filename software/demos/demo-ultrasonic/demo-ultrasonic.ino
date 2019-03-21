#include <Ultrasonic.h>

int trigPin = 11;
int echoPin = 12;
long duration, cm;

Ultrasonic US(trigPin, echoPin);

void setup() {
  //Serial Port begin
  Serial.begin (9600);
  Serial.print("Starting...");
}

int num_tiles;

void loop() {
  Serial.print(US.ReadNumTiles(10000, num_tiles));
  Serial.print(" status, ");
  Serial.print(num_tiles);
  Serial.print(" tiles");
  Serial.println();

  delay(16);
}
