#include <Navigator.h>

#define IMU_0360_DELAY 100

const PIDSettings PIDTurnLeft = {
    1.8,      // kp
    0.001,   // ki
    5,      // kd
    0,     // outputMin
    120,    // outputMax
    10,   // tolerance
};
const PIDSettings PIDTurnRight = {
    1.8,      // kp
    0.001,   // ki
    5,      // kd
    0,     // outputMin
    120,    // outputMax
    9,   // tolerance
};

void printTargetAnglePWM(float target, float angle, float pwm) {
    static char msg[64];
    static char floats[3][8];
    
    dtostrf(target, 7, 3, floats[0]);
    dtostrf(angle, 7, 3, floats[1]);
    dtostrf(pwm, 7, 3, floats[2]);
    snprintf(msg, 64, "Target Angle: %s, Angle: %s, PWM: %s", floats[0], floats[1], floats[2]);
    Serial.println(msg);
}

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

inline void Navigator::_turnLeftMotorCommand(float pwm) {
    motors.TurnLeft(pwm, pwm - 5, pwm, pwm - 5);
}

inline void Navigator::_turnRightMotorCommand(float pwm) {
    motors.TurnRight(pwm - 3, pwm, pwm - 3, pwm);
}

void Navigator::turnLeft() {
    sensors_event_t s;
    Direction nextDirection = Navigator::_getNextDirectionLeft(currentDirection);

    pid.clearPreviousResults();
    pid.setSetpoint(Navigator::_getAngleFromDirection(nextDirection, currentDirection), PIDTurnLeft.tolerance);
    _turnLeftMotorCommand(PIDTurnLeft.outputMax);

    if (currentDirection == North) {
        int count = 0;
        do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            if (s.orientation.x > 260)
                count++;
            else
                count = 0;
        } while (count < 3);
    }

    if (nextDirection == North) {
        // Handle the back-and-forth between 0 degrees and 360 degrees
        do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            if (s.orientation.x > 300)
                break;
            float pwm = pid.compute(s.orientation.x);
            if (pwm < 0)
                _turnLeftMotorCommand(abs(pwm));
        } while (!pid.isLessThanSetpoint());
    } else {
        do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            float pwm = pid.compute(s.orientation.x);
            if (pwm < 0)
                _turnLeftMotorCommand(abs(pwm));

#ifdef Navigator_Debug_mode
            printTargetAnglePWM(-1, s.orientation.x, pwm);
            Serial.println(pwm);
            Serial.println(s.orientation.x);
#endif
        } while (!pid.isLessThanSetpoint());
    }

    motors.Halt();
    currentDirection = nextDirection;
    imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
    Serial.println(s.orientation.x);
}

void Navigator::_turnLeftDebug() {
    char debug[128];
    sensors_event_t s;
    Direction nextDirection = Navigator::_getNextDirectionLeft(currentDirection);
    float targetAngle = Navigator::_getAngleFromDirection(nextDirection, currentDirection);

    if (targetAngle < 0)
        Serial.println("Target angle error");

    pid.clearPreviousResults();
    pid.setSetpoint(targetAngle, PIDTurnLeft.tolerance);

    if (currentDirection == North) {
        // Handle the initial IMU transition from 0 degrees to 360 degrees
        /*
        do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);

            printTargetAnglePWM(targetAngle, s.orientation.x, PIDTurnLeft.outputMax);

        } while (s.orientation.x < 350);*/
        int count = 0;
        do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);

            if (s.orientation.x > 260)
                count++;
            else
                count = 0;

        } while (count < 10);
    }

    if (nextDirection == North) {
        // Handle the back-and-forth between 0 degrees and 360 degrees
        do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            float pwm = abs(pid.compute(s.orientation.x));  // error is is negative for a left turn

            printTargetAnglePWM(targetAngle, s.orientation.x, pwm);

            if (s.orientation.x > 350)
                break;

        } while (!pid.isLessThanSetpoint());
    } else {
        do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            float pwm = abs(pid.compute(s.orientation.x));  // error is is negative for a left turn

            printTargetAnglePWM(targetAngle, s.orientation.x, pwm);

        } while (!pid.isLessThanSetpoint());
    }

    snprintf(debug, 128, "Finished turning. Previous direction: %s, current direction: %s", _getDirectionAsString(currentDirection).c_str(), _getDirectionAsString(nextDirection).c_str());
    Serial.println(debug);
    currentDirection = nextDirection;
}

void Navigator::turnRight() {
    sensors_event_t s;
    Direction nextDirection = Navigator::_getNextDirectionRight(currentDirection);

    pid.clearPreviousResults();
    pid.setSetpoint(Navigator::_getAngleFromDirection(nextDirection, currentDirection), PIDTurnRight.tolerance);
    _turnRightMotorCommand(PIDTurnRight.outputMax);

    if (currentDirection == North) {
        // Handle the initial IMU transition from 360 degrees to 0 degrees
        //delay(IMU_0360_DELAY);
        int count = 0;
        do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);

            if (s.orientation.x < 100)
                count++;
            else
                count = 0;

        } while (count < 2);
    }

    if (nextDirection == North) {
        // Handle the back-and-forth between 360 degrees and 0 degrees
        do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            if (s.orientation.x < 100)
                break;
            float pwm = pid.compute(s.orientation.x);

            if (pwm > 0)
                _turnRightMotorCommand(pwm);

        } while (!pid.isGreaterThanSetpoint());
    } else {
        do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            float pwm = pid.compute(s.orientation.x);

            if (pwm > 0)
                _turnRightMotorCommand(pwm);

        } while (!pid.isGreaterThanSetpoint());
    }

    motors.Halt();
    currentDirection = nextDirection;

    imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
    Serial.println(s.orientation.x);
}

void Navigator::_turnRightDebug() {
    char debug[128];
    sensors_event_t s;
    Direction nextDirection = Navigator::_getNextDirectionRight(currentDirection);
    float targetAngle = Navigator::_getAngleFromDirection(nextDirection, currentDirection);

    if (targetAngle < 0)
        Serial.println("Target angle error");

    pid.clearPreviousResults();
    pid.setSetpoint(targetAngle, PIDTurnRight.tolerance);

    if (currentDirection == North) {
        // Handle the initial IMU transition from 360 degrees to 0 degrees
        /*do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);

            printTargetAnglePWM(targetAngle, s.orientation.x, PIDTurnRight.outputMax);

        } while (s.orientation.x > 10);*/
        int count = 0;
        do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);

            if (s.orientation.x < 100)
                count++;
            else
                count = 0;

        } while (count < 10);
    }

    if (nextDirection == North) {
        // Handle the back-and-forth between 360 degrees and 0 degrees
        do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            if (s.orientation.x < 10)
                break;
            float pwm = pid.compute(s.orientation.x);

            printTargetAnglePWM(targetAngle, s.orientation.x, pwm);

        } while (!pid.isGreaterThanSetpoint());
    } else {
        do {
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            float pwm = pid.compute(s.orientation.x);

            printTargetAnglePWM(targetAngle, s.orientation.x, pwm);

        } while (!pid.isGreaterThanSetpoint());
    }

    snprintf(debug, 128, "Finished turning. Previous direction: %s, current direction: %s", _getDirectionAsString(currentDirection).c_str(), _getDirectionAsString(nextDirection).c_str());
    Serial.println(debug);
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

Direction Navigator::getCurrentDirection() {
    return currentDirection;
}

String Navigator::_getDirectionAsString(Direction d) {
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

static float Navigator::_getAngleFromDirection(Direction d, Direction prevd) {
    switch (d) {
        case North:
            if (prevd == West)
                return 360;
            if (prevd == East)
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

void Navigator::manualMode(bool debugMode) {
    enum KeyState {
        W,
        A,
        S,
        D,
        Q,
    };

    KeyState ks = W;
    char key;

    debugMode ? Serial.println("Entered manual mode [DEBUG]") : Serial.println("Entered manual mode");

    while (1) {
        if (Serial.available() > 0) {
            key = Serial.read();
            switch (key) {
                case 'w':
                    if (ks != W) {
                        halt();
                        Serial.println("Going forward");
                        delay(1000);
                        if (!debugMode)
                            goForward();
                        ks = W;
                    }
                    break;
                case 'a':
                    halt();
                    Serial.print("Turning left...");
                    delay(1000);
                    debugMode ? _turnLeftDebug() : turnLeft();
                    ks = Q;
                    Serial.println("Done");
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
                    debugMode ? _turnRightDebug() : turnRight();
                    ks = Q;
                    break;
                case 'q':
                    if (ks != Q) {
                        Serial.println("Stopping");
                        if (!debugMode)
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
                default:
                    break;
            }
        }
        delay(50);
    }
}

void Navigator::imuLoop() {
    Serial.println("Entered IMU loop");
    delay(2000);
    while (true) {
        sensors_event_t s;
        imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
        Serial.println(s.orientation.x);
        delay(100);
    }
}
