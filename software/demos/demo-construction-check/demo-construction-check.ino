#include <Motors.h>
#include <Flame.h>
//#include <Ultrasonic.h>
#include <TFLidar.h>

Motors motors;
// Ultrasonic ultrasonic();
TFLidar lidar(8);
Flame flame(A0, A1);

enum class MotorStatus {
    STOPPED,
    FORWARD,
    TURNING_LEFT,
    TURNING_RIGHT,
    SLIGHT_LEFT,
    SLIGHT_RIGHT
};

MotorStatus motorStatus;

void setup() {
    Serial.begin(9600);
    lidar.begin(&Serial3);
    pinMode(52, INPUT_PULLUP);
    motorStatus = MotorStatus::STOPPED;
}

void loop() {
    Serial.println("Starting code");
    while (digitalRead(52)) {
        TFStatus st = lidar.measure();
        // lidar.printLatest(&Serial, true);
        // Serial.println(flame.readValue(true));
        delay(100);
        lidar.measure();
        Serial.println(lidar.getDistanceRaw());
    }
    Serial.println("Read pin");
    delay(1000);
    while (true) {
        delay(50);
        TFStatus st = lidar.measure();
        uint16_t lidar_distance = lidar.getDistanceRaw();

        if (lidar_distance > 36 && motorStatus != MotorStatus::FORWARD) {
            motorStatus = MotorStatus::FORWARD;
            motors.Halt();
            motors.DriveFwd();
        } else if (lidar_distance <= 45) {
            motorStatus = MotorStatus::STOPPED;
            motors.Halt();
            motors.DriveRev();
            delay(3000);
            motors.Halt();

            // wait for flame
            while (flame.readValue(true) > 400) {}
            delay(1000);

            motors.TurnLeft();
            delay(2000);
            motors.Halt();
            motors.TurnRight();
            delay(2000);
            motors.Halt();
            while (true); {}
            // int ultrasonicDist = ultrasonic.getDistance();
            // if (ultrasonicDist > 5 && motorStatus != MotorStatus::FORWARD) {
            //     motorStatus = MotorStatus::FORWARD;
            //     motors.Halt();
            //     motors.DriveFwd();
            // } else if (ultrasonicDist <= 5 && MotorStatus::TURNING_LEFT) {
            //     motorStatus = MotorStatus::TURNING_LEFT;
            //     motors.Halt();
            //     motors.TurnLeft();
            // }
        }
    }
}

// void setup() {
//     Serial.begin(9600);
//     motorStatus = MotorStatus::STOPPED;
// }

// void loop() {
//     FlameLocation flameLocation;
//     flameLocation = flame.locate();
//     if (flameLocation == FLAME_IS_CENTERED && MotorStatus != motorStatus::FORWARD) {
//         motorStatus = MotorStatus::FORWARD;
//         motors.Halt();
//         motors.DriveFwd();
//     } else if (flameLocation == FLAME_IS_LEFT && motorStatus != MotorStatus::TURNING_LEFT) {
//         motorStatus = MotorStatus::TURNING_LEFT;
//         motors.Halt();
//         motors.TurnLeft();
//     } else if (flameLocation == FLAME_IS_RIGHT && motorStaus != MotorStaus::TURNING_RIGHT) {
//         motorStatus = MotorStatus::TURNING_RIGHT;
//         motors.Halt();
//         motors.TurnRight();
//     }
// }
