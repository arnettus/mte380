#include  "Gravity.h"
#include "Wire.h"

GravitySensors gravities;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("test setup");

  gravities.Init();
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  gravities.PrintRaw();
//  gravities.PrintTile();
}
