#include  "Gravity.h"
#include "Wire.h"

GravityConfig config1 = {.capture = GravityConfig::NONE,
                         .range = GravityConfig::STANDARD,
                         .address = 0x29,
                         .reset_pin = 4,
                         .timeout = 500,
                         .capacity = 10,
                         .min_range = 60,
                         .max_range = 400
};

Gravity gravity(config1);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("test setup");
  gravity.Init();
  // put your setup code here, to run once:
}

uint16_t distance = 0;

void loop() {
  // put your main code here, to run repeatedly:
  Gravity::Status status = gravity.GetAndCheckDistance(distance);
  Serial.print(status);
  Serial.print(" ");
  Serial.println(distance);
}
