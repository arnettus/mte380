#ifndef Navigator_H
#define Navigator_H

#include <Motors.h>
#include <PID.h>
#include <Adafruit_BNO055.h>
#include <TFMiniLidar.h>

enum Direction {
    North,
    East,
    South,
    West,
    Nothing
};

class Navigator {
public:
    Navigator();
    bool begin();
    void turnLeft();
    void turnRight();
    void goForward(uint16_t targetDistance);
    void goReverse(uint16_t targetDistance);
    void halt();

    void readMagnetometer();
    bool detectedMagnet();

    void demoManualMode();
    void demoIMULoopOrientation();
    void demoIMULoopMagnetometer();

    void calibrateIMU();
    void printIMUOffsets();

private:
    Direction       currentDirection;
    Adafruit_BNO055 imu;
    PID             pidl;
    PID             pidr;
    PID             pidfwd;
    PID             pidrev;
    Motors          motors;

    static Direction _getNextDirectionLeft(Direction d);
    static Direction _getNextDirectionRight(Direction d);
    static float _getAngleFromDirection(Direction d, bool use360Not0);
    static String _getDirectionAsString(Direction d);
    inline void _turnLeftMotorCommand(float speed);
    inline void _turnRightMotorCommand(float speed);
    inline void _goForwardMotorCommand(float speed);
    inline void _goReverseMotorCommand(float speed);
};

#endif
