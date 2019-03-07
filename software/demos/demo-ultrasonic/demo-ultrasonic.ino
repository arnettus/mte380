//#include <Ultrasonic.h>
//
//Ultrasonic u(13, 12);
//
//void setup() {
//    Serial.begin(9600);
//
//    noInterrupts();
//    OCR1A = 0xFFFF;   // approximately every 16ms
//
//    TCCR1A = 0;
//    TCCR1B = (1 << WGM12)|(1 << CS11)|(1 << CS10);
//    TCNT1 = 0;
//
//    TIMSK1 |= _BV(OCIE1A);
//    interrupts();
//}
//
//volatile bool pollMe = false;
//
//ISR(TIMER1_COMPA_vect) {
//    if (!pollMe)
//        pollMe = true;
//}
//
//void loop() {
//   if (pollMe) {
//        Serial.println(u.ReadDistance());
//        pollMe = false;
//    }
//}

int trigPin = 13;
int echoPin = 12;
long duration, cm;

void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.print("Starting...");
}

void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343

  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(16);
}
