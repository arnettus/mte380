//  Motors.cpp - Library for Cytron quad-drive
#define ENCODER_OPTIMIZE_INTERRUPTS
#include "Encoder.h"
#include "Arduino.h"
#include "Motors.h"

// Motor control pins
#define M1_DIR  2
#define M1_EN   3
#define M2_DIR  4
#define M2_EN   5
#define M3_DIR  6
#define M3_EN   7
#define M4_DIR  8
#define M4_EN   9

// Encoder pins must be placed on Arduino Mega interrupt pins
// Valid pins: 2, 3, 18, 19, 20, 21
#define M1_ENCA 18
#define M1_ENCB 19
#define M2_ENCA 20
#define M2_ENCB 21

// ================================== Motor 1 ==================================
void Motors::_DriveFrontRightFWD(int val = 58){
    digitalWrite(M1_DIR, LOW); // CCW rotation, left motor goes FWD
    analogWrite(M1_EN, val);
}

void Motors::_DriveFrontRightREV(int val = 56){
    digitalWrite(M1_DIR, HIGH); // CW rotation, left motor goes REV
    analogWrite(M1_EN, val);
}

// ================================== Motor 2 ==================================
void Motors::_DriveFrontLeftFWD(int val = 60){
    digitalWrite(M2_DIR, HIGH); // CW rotation, right motor goes FWD
    analogWrite(M2_EN, val);
}

void Motors::_DriveFrontLeftREV(int val = 60){
    digitalWrite(M2_DIR, LOW); // CCW rotation, right motor goes REV
    analogWrite(M2_EN, val);
}

// ================================== Motor 3 ==================================
void Motors::_DriveRearRightREV(int val = 64){
    digitalWrite(M3_DIR, LOW); // CCW rotation, left motor goes FWD
    analogWrite(M3_EN, val);
}

void Motors::_DriveRearRightFWD(int val = 66){
    digitalWrite(M3_DIR, HIGH); // CW rotation, left motor goes REV
    analogWrite(M3_EN, val);
}

// ================================== Motor 4 ==================================
void Motors::_DriveRearLeftREV(int val = 63){
    digitalWrite(M4_DIR, HIGH); // CW rotation, right motor goes FWD
    analogWrite(M4_EN, val);
}

void Motors::_DriveRearLeftFWD(int val = 66){
    digitalWrite(M4_DIR, LOW); // CCW rotation, right motor goes REV
    analogWrite(M4_EN, val);
}

Motors::Motors(): _m1Enc(M1_ENCA, M1_ENCB), _m2Enc(M2_ENCA, M2_ENCB){
    pinMode(M1_DIR, OUTPUT);
    pinMode(M1_EN, OUTPUT);
    pinMode(M2_DIR, OUTPUT);
    pinMode(M2_EN, OUTPUT);
    pinMode(M3_DIR, OUTPUT);
    pinMode(M3_EN, OUTPUT);
    pinMode(M4_DIR, OUTPUT);
    pinMode(M4_EN, OUTPUT); 

    _busy = false;
    // _m1Enc = Encoder(M1_ENCA, M1_ENCB);
    // _m2Enc = Encoder(M2_ENCA, M2_ENCB);
}

void Motors::DriveFwd(){
    if(!_busy){
        // Take the lock
        _busy = true;
        // Front motors FWD
        Motors::_DriveFrontLeftFWD();
        Motors::_DriveFrontRightFWD();
        // Rear motors FWD
        Motors::_DriveRearLeftFWD();
        Motors::_DriveRearRightFWD();
    }
}

void Motors::DriveRev(){
    if(!_busy){
        // Take the lock
        _busy = true;
        // Front motors FWD
        // Front motors FWD
        Motors::_DriveFrontLeftREV();
        Motors::_DriveFrontRightREV();
        // Rear motors FWD
        Motors::_DriveRearLeftREV();
        Motors::_DriveRearRightREV();
    }
}

void Motors::Halt(){
    if(_busy){
        // Set all speeds to 0
        analogWrite(M1_EN, 0);
        analogWrite(M2_EN, 0);
        analogWrite(M3_EN, 0);
        analogWrite(M4_EN, 0);
        // Release the lock
        _busy = false;
    }
}

void Motors::TurnRight(){
    if(!_busy){
        // Take the lock
        _busy = true;
        // Left side motors FWD
        Motors::_DriveFrontLeftREV(90);
        Motors::_DriveRearLeftREV(90);
        // Right side motors REV
        Motors::_DriveFrontRightFWD(90);
        Motors::_DriveRearRightFWD(90);
    }
}

void Motors::TurnLeft(){
    if(!_busy){
        // Take the lock
        _busy = true;
        // Left side motors FWD
        Motors::_DriveFrontLeftFWD(90);
        Motors::_DriveRearLeftFWD(90);
        // Right side motors REV
        Motors::_DriveFrontRightREV(90);
        Motors::_DriveRearRightREV(90);
    }
}

int Motors::GetMotor1Enc(){
    return(_m1Enc.read());
}

int Motors::GetMotor2Enc(){
    return(_m2Enc.read());
}

void Motors::SetMotor1Enc(int val){
    _m1Enc.write(val);
}

void Motors::SetMotor2Enc(int val){
    _m2Enc.write(val);
}