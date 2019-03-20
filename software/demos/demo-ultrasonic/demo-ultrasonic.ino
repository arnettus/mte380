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

void loop() {
  Serial.print(US.ReadAverageDistance(1000));
  Serial.print("cm");
  Serial.println();

  delay(16);
}
