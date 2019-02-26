#include "Motors.h"

Motors myMotors;

void setup()
{
  Serial.begin(9600);
  Serial.println("Library Testing...");
}

void loop()
{
  myMotors.Halt();
  delay(3000);
  myMotors.DriveFwd();
  delay(3000);
  myMotors.Halt();
  myMotors.DriveRev();
  delay(3000);
  myMotors.Halt();
  myMotors.TurnLeft();
  delay(3000);
  myMotors.Halt();
  myMotors.TurnRight();
  delay(3000);
  Serial.println(myMotors.GetMotor1Enc());
  Serial.println(myMotors.GetMotor2Enc());
  myMotors.SetMotor1Enc(0);
  myMotors.SetMotor2Enc(0);
}