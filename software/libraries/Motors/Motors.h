#ifndef Motors_h
#define Motors_h

#include "Arduino.h"
#include "Encoder.h"

class Motors
{
    public:
        Motors();
        void DriveFwd();
        void DriveRev();
        void TurnLeft();
        void TurnRight();
        void TurnRightSlowly();
        void TurnLeftSlowly();
        void Halt();
        void TurnRightSand();
        void TurnLeftSand();
        void OLTurnLeft90();
        void OLTurnRight90();
        int GetMotor1Enc();
        void SetMotor1Enc(int val);
        int GetMotor2Enc();
        void SetMotor2Enc(int val);
    private:
        Encoder _m1Enc;
        Encoder _m2Enc;
        bool _busy;
        void _DriveFrontLeftFWD(int val);
        void _DriveFrontLeftREV(int val);
        void _DriveFrontRightFWD(int val);
        void _DriveFrontRightREV(int val);
        void _DriveRearLeftFWD(int val);
        void _DriveRearLeftREV(int val);
        void _DriveRearRightFWD(int val);
        void _DriveRearRightREV(int val);
};

#endif