#include <Wire.h>
#include <VL53L0X.h>
#include <string.h>

#define ADDRESS_1 0x31
#define ADDRESS_2 0x32
#define ADDRESS_3 0x33
#define ADDRESS_4 0x34

VL53L0X g1;
VL53L0X g2(23);
VL53L0X g3(25);
VL53L0X g4(27);

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

#define LONG_RANGE

// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
//#define HIGH_ACCURACY

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  g2.writeResetPin(true);
  delay(500);
  
  g3.writeResetPin(true);
  delay(500);
  
  g4.writeResetPin(true);
  delay(500);

  g1.init();
  g1.setTimeout(500);
  g1.setAddress(ADDRESS_1);

  delay(500);
  g2.writeResetPin(false);
  delay(500);

  g2.init();
  g2.setTimeout(500);
  g2.setAddress(ADDRESS_2);

  delay(500);
  g3.writeResetPin(false);
  delay(500);

  g3.init();
  g3.setTimeout(500);
  g3.setAddress(ADDRESS_3);

  delay(500);
  g4.writeResetPin(false);
  delay(500);

  g4.init();
  g4.setTimeout(500);
  g4.setAddress(ADDRESS_4);

  delay(500);

  Serial.println("Initialization complete");

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  g1.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  g1.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  g1.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  g1.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  g1.setMeasurementTimingBudget(200000);
#endif
}

uint16_t values[4] = {0};
char msg[50];

void loop()
{
  values[0] = g1.readRangeSingleMillimeters();
  values[1] = g2.readRangeSingleMillimeters();
  values[2] = g3.readRangeSingleMillimeters();
  values[3] = g4.readRangeSingleMillimeters();

  snprintf(msg, 50, "g1=%u, g2=%u, g3=%u, g4=%u", values[0], values[1], values[2], values[3]);

  Serial.println(msg);
  delay(100);
}
