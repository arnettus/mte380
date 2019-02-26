#ifndef Motors_h
#define Motors_h

#include "Arduino.h"
#include "src/Encoder/Encoder.h"

class Motors
{
    public:
        Motors();
        void DriveFwd();
        void DriveRev();
        void TurnRight();
        void TurnLeft();
        void Halt();
        int GetMotor1Enc();
        void SetMotor1Enc(int val);
        int GetMotor2Enc();
        void SetMotor2Enc(int val);
    private:
        Encoder _m1Enc;
        Encoder _m2Enc;
        bool _busy;
        void _DriveFrontLeftFWD();
        void _DriveFrontLeftREV();
        void _DriveFrontRightFWD();
        void _DriveFrontRightREV();
        void _DriveRearLeftFWD();
        void _DriveRearLeftREV();
        void _DriveRearRightFWD();
        void _DriveRearRightREV();
};

#endif