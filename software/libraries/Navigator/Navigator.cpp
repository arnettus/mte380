#include <Navigator.h>

const PIDSettings PIDTurnLeft = {
    100,    // dt in ms
    3,      // kp
    0.01,   // ki
    0,      // kd
    90,     // outputMin
    210,    // outputMax
    0.01,   // tolerance
};

Navigator::Navigator()
    : currentDirection(North)
    , pid(PIDTurnLeft)
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

// Takes a single pwm command and produces relative motor speeds based on experimental results
inline void Navigator::_turnLeftMotorCommand(float pwm) {
    motors.TurnLeft(pwm, pwm - 3, pwm, pwm - 3);
}

// Takes a single speed command and produces relative motor speeds based on experimental results
inline void Navigator::_turnRightMotorCommand(float pwm) {
    motors.TurnRight(pwm - 3, pwm, pwm - 3, pwm);
}

void Navigator::turnLeft() {
    sensors_event_t s;
    Direction nextDirection = _getNextDirectionLeft();

    pid.clearPreviousResults();
    pid.setSetpoint(Navigator::_getAngleFromDirection(nextDirection), PIDTurnLeft.tolerance);
    _turnLeftMotorCommand(PIDTurnLeft.outputMax);

    if (currentDirection == North) {
        // Handle the initial IMU transition from 0 degrees to 360 degrees
        do {
            delay(PIDTurnLeft.dt);
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
        } while (s.orientation.x < 350);
    }

    if (nextDirection == North) {
        // Handle the back-and-forth between 0 degrees and 360 degrees
        do {
            delay(PIDTurnLeft.dt);
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            float pwm = abs(pid.compute(s.orientation.x));  // error is is negative for a left turn
            if (s.orientation.x > 350)
                break;
            _turnLeftMotorCommand(pwm);
        } while (!pid.hasReachedSetpoint());
    } else {
        do {
            delay(PIDTurnLeft.dt);
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            float pwm = abs(pid.compute(s.orientation.x));  // error is is negative for a left turn
            _turnLeftMotorCommand(pwm);
        } while (!pid.hasReachedSetpoint());
    }

    currentDirection = nextDirection;
}

String _getDirectionAsString(Direction d) {
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

void Navigator::_turnLeftDebug() {
    char debug[64];

    sensors_event_t s;
    Direction nextDirection = _getNextDirectionLeft();

    float targetAngle = Navigator::_getAngleFromDirection(nextDirection);

    pid.clearPreviousResults();
    pid.setSetpoint(targetAngle, PIDTurnLeft.tolerance);

    if (currentDirection == North) {
        // Handle the initial IMU transition from 0 degrees to 360 degrees
        do {
            delay(PIDTurnLeft.dt);
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            
            snprintf(debug, 64, "Target Angle: %d, Angle: %d", targetAngle, s.orientation.x);
            Serial.println(debug);

        } while (s.orientation.x < 350);
    }

    if (nextDirection == North) {
        // Handle the back-and-forth between 0 degrees and 360 degrees
        do {
            delay(PIDTurnLeft.dt);
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            float pwm = abs(pid.compute(s.orientation.x));  // error is is negative for a left turn

            snprintf(debug, 64, "Target Angle: %d, Angle: %d, PWM: %d", targetAngle, s.orientation.x, pwm);
            Serial.println(debug);

            if (s.orientation.x > 350)
                break;

        } while (!pid.hasReachedSetpoint());
    } else {
        do {
            delay(PIDTurnLeft.dt);
            imu.getEvent(&s, Adafruit_BNO055::VECTOR_EULER);
            float pwm = abs(pid.compute(s.orientation.x));  // error is is negative for a left turn

            snprintf(debug, 64, "Angle: %d, PWM: %d", s.orientation.x, pwm);
            Serial.println(debug);

        } while (!pid.hasReachedSetpoint());
    }

    snprintf(debug, 64, "Previous direction: %s, current direction: %s", _getDirectionAsString(currentDirection), _getDirectionAsString(nextDirection));

    currentDirection = nextDirection;
}

void Navigator::turnRight() {

}

void Navigator::_turnRightDebug() {

}

void Navigator::goForward() {

}

void Navigator::goReverse() {

}

void Navigator::halt() {

}

Direction Navigator::getCurrentDirection() {
    return currentDirection;
}

Direction Navigator::_getNextDirectionLeft() {
    if (currentDirection == North)
        return West;
    return static_cast<Direction>(currentDirection - 1);
}

Direction Navigator::_getNextDirectionRight() {
    if (currentDirection == West)
        return North;
    return static_cast<Direction>(currentDirection + 1);
}

static float Navigator::_getAngleFromDirection(Direction d) {
    switch (d) {
        case North:
            return 0;
        case East:
            return 90;
        case South:
            return 180;
        case West:
            return 270;
    }
    return 0;
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
                        halt();
                        ks = Q;
                    }
                    break;
                default:
                    break;
            }
        }
        delay(50);
    }
}

