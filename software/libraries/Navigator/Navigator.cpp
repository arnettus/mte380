#include <Navigator.h>

void printTargetAngleSpeed(float target, float angle, float speed);

const PIDSettings PIDTurnLeft = {
    .kp = 1.8,
    .ki = 0.00005,
    .kd = 4,
    .outputMin = 80,
    .outputMax = 240,
    .tolerance = 0.2,
    .useKpOnMeasure = false,
};

const PIDSettings PIDTurnRight = {
    .kp = 1.7,
    .ki = 0.0001,
    .kd = 3.8,
    .outputMin = 80,
    .outputMax = 240,
    .tolerance = 0.2,
    .useKpOnMeasure = false,
};

Navigator::Navigator()
    : currentDirection(North)
    , pidl(PIDTurnLeft)
    , pidr(PIDTurnRight)
    , imu(55)
{
}

bool Navigator::begin() {
    if (!imu.begin()) {
        Serial.println("Could not find the BNO055");
        return false;
    }
    return true;
}

void Navigator::turnLeft() {
    sensors_event_t s;
    Direction nextDirection = Navigator::_getNextDirectionLeft(currentDirection);
    float targetAngle       = Navigator::_getAngleFromDirection(nextDirection, false);

    imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);

    // Handle the 0-360 boundary
    if (currentDirection == North) {
        float currentAngle = s.orientation.x;

        if (currentAngle < 180)
            currentAngle += 360;
        
        pidl.begin(targetAngle, currentAngle);
        float speed = 0;

        while (!pidl.isLessThanSetpoint()) {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            currentAngle = s.orientation.x;
            if (currentAngle < 180)
                currentAngle += 360;

            speed = pidl.compute(currentAngle);
            printTargetAngleSpeed(targetAngle, currentAngle, speed);

            if (speed < 0) {
                _turnLeftMotorCommand(abs(speed));
            }
            //else
            //    _turnRightMotorCommand(speed);
        }
    } else {
        pidl.begin(targetAngle, s.orientation.x);
        float speed = 0;

        while (!pidl.isLessThanSetpoint()) {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);

            if (nextDirection == North && s.orientation.x > 180) {  // Handle the 0-360 boundary
                Serial.println(s.orientation.x);
                break;
            }

            speed = pidl.compute(s.orientation.x);
            printTargetAngleSpeed(targetAngle, s.orientation.x, speed);

            if (speed < 0)
                _turnLeftMotorCommand(abs(speed));
            //else
            //    _turnRightMotorCommand(speed);
        }
    }

    halt();
    Serial.print("Stopped at ");
    Serial.println(s.orientation.x);
    currentDirection = nextDirection;
}

void Navigator::turnRight() {
    sensors_event_t s;
    Direction nextDirection = Navigator::_getNextDirectionRight(currentDirection);
    float targetAngle       = Navigator::_getAngleFromDirection(nextDirection, true);

    imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);

    // Handle the 0-360 boundary
    if (currentDirection == North) {
        float currentAngle = s.orientation.x;
        if (currentAngle > 180)
            currentAngle = currentAngle - 360;

        pidr.begin(targetAngle, currentAngle);
        float speed = 0;

        while (!pidr.isGreaterThanSetpoint()) {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            currentAngle = s.orientation.x;

            if (currentAngle > 180)
                currentAngle = currentAngle - 360;

            speed = pidr.compute(currentAngle);
            printTargetAngleSpeed(targetAngle, currentAngle, speed);

            if (speed > 0)
                _turnRightMotorCommand(abs(speed));
            else
                _turnLeftMotorCommand(speed);
        }
    } else {
        pidr.begin(targetAngle, s.orientation.x);
        float speed = 0;

        while (!pidr.isGreaterThanSetpoint()) {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);

            if (nextDirection == North && s.orientation.x < 180) {    // Handle the 0-360 boundary
                Serial.println(s.orientation.x);
                break;
            }
            
            speed = pidr.compute(s.orientation.x);
            printTargetAngleSpeed(targetAngle, s.orientation.x, speed);

            if (speed > 0)
                _turnRightMotorCommand(speed);
            else
                _turnLeftMotorCommand(abs(speed));
        }
    }

    halt();
    currentDirection = nextDirection;
}

void Navigator::goForward() {
    motors.DriveFwd();
}

void Navigator::goReverse() {
    motors.DriveRev();
}

void Navigator::halt() {
    motors.Halt();
}

void Navigator::readMagnetometer() {
    char f[3][8];
    char msg[64];
    sensors_event_t s;
    imu.getEvent(&s, Adafruit_BNO055::VECTOR_MAGNETOMETER);

    dtostrf(s.magnetic.x, 7, 3, f[0]);
    dtostrf(s.magnetic.y, 7, 3, f[1]);
    dtostrf(s.magnetic.z, 7, 3, f[2]);

    snprintf(msg, 64, "x: %s, y: %s, z: %s", f[0], f[1], f[2]);
    Serial.println(msg);
}

inline void Navigator::_turnLeftMotorCommand(float speed) {
    motors.TurnLeft(speed, speed - 5, speed, speed - 5);
}

inline void Navigator::_turnRightMotorCommand(float speed) {
    motors.TurnRight(speed - 3, speed, speed - 3, speed);
}

static String Navigator::_getDirectionAsString(Direction d) {
    switch (d) {
        case North:
            return "North";
        case East:
            return "East";
        case South:
            return "South";
        case West:
            return "West";
        default:
            return "";
    }
}

static Direction Navigator::_getNextDirectionLeft(Direction d) {
    if (d == North)
        return West;
    return static_cast<Direction>(d - 1);
}

static Direction Navigator::_getNextDirectionRight(Direction d) {
    if (d == West)
        return North;
    return static_cast<Direction>(d + 1);
}

static float Navigator::_getAngleFromDirection(Direction d, bool use360Not0) {
    switch (d) {
        case North:
            if (use360Not0)
                return 360;
            else
                return 0;
        case East:
            return 90;
        case South:
            return 180;
        case West:
            return 270;
    }
    return -1;
}

void Navigator::calibrateIMU() {
    char msg[64];
    uint8_t sys, gyro, accel, mag;

    Serial.println("Calibrating the IMU...");
    imu.getCalibration(&sys, &gyro, &accel, &mag);

    while ((sys != 3 || gyro != 3 || mag != 3 || accel != 3)) {
        delay(2000);
        imu.getCalibration(&sys, &gyro, &accel, &mag);

        snprintf(msg, 64, "sys: %u, gyro: %u, accel: %u, mag: %u", sys, gyro, accel, mag);
        Serial.println(msg);
    }

    Serial.println("IMU has been calibrated");
}

void Navigator::printIMUOffsets() {
    char msg[128];
    adafruit_bno055_offsets_t offsets;
    bool st = imu.getSensorOffsets(offsets);

    if (!st)
        Serial.println("Unable to read the offsets");

    snprintf(msg, 128, "a_x: %d, a_y: %d, a_z: %d, a_r: %d\ng_x: %d, g_y: %d, g_z: %d\nm_x: %d, m_y: %d:, m_z: %d, m_r: %d\n", 
            offsets.accel_offset_x, offsets.accel_offset_y, offsets.accel_offset_z, offsets.accel_radius, 
            offsets.gyro_offset_x, offsets.gyro_offset_y, offsets.gyro_offset_z,
            offsets.mag_offset_x, offsets.mag_offset_y, offsets.mag_offset_z, offsets.mag_radius); 

    Serial.println(msg);
}


void Navigator::demoManualMode() {
    enum KeyState {
        W,
        A,
        S,
        D,
        Q,
        F,
    };

    KeyState ks = Q;
    char key;

    Serial.println("Entered manual mode.");

    while (1) {
        if (Serial.available() > 0) {
            key = Serial.read();
            switch (key) {
                case 'w':
                    if (ks != W) {
                        halt();
                        Serial.println("Going forward");
                        delay(1000);
                        goForward();
                        ks = W;
                    }
                    break;
                case 'a':
                    halt();
                    Serial.print("Turning left");
                    delay(1000);
                    turnLeft();
                    ks = Q;
                    break;
                case 's':
                    if (ks != S) {
                        halt();
                        Serial.println("Going reverse");
                        delay(1000);
                        goReverse();
                        ks = S;
                    }
                    break;
                case 'd':
                    halt();
                    Serial.println("Turning right");
                    delay(1000);
                    turnRight();
                    ks = Q;
                    break;
                case 'q':
                    if (ks != Q) {
                        Serial.println("Stopping");
                        halt();
                        ks = Q;
                    }
                    break;
                case 'x':
                    sensors_event_t s;
                    imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
                    Serial.print("Current direction: ");
                    Serial.print(Navigator::_getDirectionAsString(currentDirection));
                    Serial.print(", Orientation: ");
                    Serial.println(s.orientation.x);
                case 'f':
                    readMagnetometer();
                default:
                    break;
            }
        }
        delay(50);
    }
}

void Navigator::demoIMULoopOrientation() {
    Serial.println("Entered IMU loop for orientation");
    delay(100);
    while (true) {
        sensors_event_t s;
        imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
        Serial.println(s.orientation.x);
        delay(100);
    }
}

void Navigator::demoIMULoopMagnetometer() {
    Serial.println("Entered IMU loop for magnetometer");
    delay(100);
    while (true) {
        readMagnetometer();
    }
}

void printTargetAngleSpeed(float target, float angle, float speed) {
    static char msg[64];
    static char floats[3][8];

    dtostrf(target, 7, 3, floats[0]);
    dtostrf(angle, 7, 3, floats[1]);
    dtostrf(speed, 7, 3, floats[2]);
    snprintf(msg, 64, "Target Angle: %s, Angle: %s, Speed: %s", floats[0], floats[1], floats[2]);
    Serial.println(msg);
}
