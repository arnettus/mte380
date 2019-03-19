#ifndef Navigator_H
#define Navigator_H

#include <Motors.h>
#include <PID.h>
#include <Adafruit_BNO055.h>

enum Direction {
    North,
    East,
    South,
    West,
};

class Navigator {
private:
    Direction       currentDirection;
    Adafruit_BNO055 imu;
    PID             pid;
    Motors          motors;

    static Direction _getNextDirectionLeft(Direction d);
    static Direction _getNextDirectionRight(Direction d);
    static float _getAngleFromDirection(Direction d, Direction prevd);
    String _getDirectionAsString(Direction d);
    inline void _turnLeftMotorCommand(float pwm);
    inline void _turnRightMotorCommand(float pwm);
    void _turnLeftDebug();
    void _turnRightDebug();

public:
    Navigator();
    bool begin();
    void turnLeft();
    void turnRight();
    void goForward();
    void goReverse();
    void halt();

    Direction getCurrentDirection();
    void manualMode(bool debugMode);
    void imuLoop();

    void calibrateIMU();
    void printIMUOffsets();
};

#endif
