#include "Beacon.h"
Beacon indicator(9,10,11);

void setup() {
  indicator.InitLED();
}
int state = 0;
void loop() {
  // put your main code here, to run repeatedly:

  indicator.SetColor(Beacon::BLUE);
  delay(1000);
  indicator.Off();
  delay(1000);
  indicator.SetColor(Beacon::GREEN);
  delay(1000);
  indicator.Off();
  delay(1000);
  indicator.SetColor(Beacon::RED);
  delay(1000);
  indicator.Off();
  delay(1000);

}
