#include <TFMiniLidar.h>

TFMiniLidar lidar(6);

void setup() {
    Serial.begin(9600);
    lidar.init(&Serial3);
    delay(5000);
    Serial.println("Initialised");
}

void loop() {
  lidar.start();
  Serial.println("ROLLING AVG START");
  for (int i = 0; i < 75; i++){
      Serial.print("Continuous: ");
      Serial.println(lidar.getDistance());
      delay(100);
  }
  Serial.println("Stopped reading");
  lidar.stop();
  delay(3000);
}
