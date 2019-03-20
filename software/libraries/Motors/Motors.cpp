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

    // _m1Enc = Encoder(M1_ENCA, M1_ENCB);
    // _m2Enc = Encoder(M2_ENCA, M2_ENCB);
}

void Motors::DriveFwd(){
    // Front motors FWD
    Motors::_DriveFrontLeftFWD(80); // M2
    Motors::_DriveFrontRightFWD(75); // M1
    // Rear motors FWD
    Motors::_DriveRearLeftFWD(84); // M4
    Motors::_DriveRearRightFWD(77); // M3
}

void Motors::DriveRev(){
    // Front motors FWD
    Motors::_DriveFrontLeftREV(79); // M2
    Motors::_DriveFrontRightREV(80); // M1
    // Rear motors FWD
    Motors::_DriveRearLeftREV(84); // M4
    Motors::_DriveRearRightREV(86); // M3
}

void Motors::Halt(){
    // Set all speeds to 0
    analogWrite(M1_EN, 0);
    analogWrite(M2_EN, 0);
    analogWrite(M3_EN, 0);
    analogWrite(M4_EN, 0);
}

void Motors::TurnLeft(){
    // Left side motors FWD
    Motors::_DriveFrontLeftREV(79); // M2
    Motors::_DriveRearLeftREV(84); // M4
    // Right side motors REV
    Motors::_DriveFrontRightFWD(75); // M1
    Motors::_DriveRearRightFWD(77); // M3
}

void Motors::TurnLeft(int fl, int rl, int fr, int rr) {
    // Left side motors FWD
    Motors::_DriveFrontLeftREV(fl); // M2
    Motors::_DriveRearLeftREV(rl); // M4
    // Right side motors REV
    Motors::_DriveFrontRightFWD(fr); // M1
    Motors::_DriveRearRightFWD(rr); // M3
}

void Motors::TurnRight(){
    // Left side motors FWD
    Motors::_DriveFrontLeftFWD(80); // M2
    Motors::_DriveRearLeftFWD(84); // M4
    // Right side motors REV
    Motors::_DriveFrontRightREV(80); // M1
    Motors::_DriveRearRightREV(86); // M3
}

void Motors::TurnRight(int fl, int rl, int fr, int rr) {
    // Left side motors FWD
    Motors::_DriveFrontLeftFWD(fl); // M2
    Motors::_DriveRearLeftFWD(rl); // M4
    // Right side motors REV
    Motors::_DriveFrontRightREV(fr); // M1
    Motors::_DriveRearRightREV(rr); // M3
}
