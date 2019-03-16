#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <stdio.h>
#include <Arduino.h>
#include <TFLidar.h>
#include <Flame.h>
#include <IR.h>
#include <Motors.h>

struct Robot {
    Robot(int a): Localization(motors_, lidar_(PIN_A, PIN_B), ultrasonic, flame) {

    }
    Localization localization_;
    Lidar lidar_;
    PathPlanning path_planning_;
};

enum class MotorStatus {
    STOPPED,
    FORWARD,
    TURNING_LEFT,
    TURNING_RIGHT,
    SLIGHT_LEFT,
    SLIGHT_RIGHT
};

struct RobotStatus {
    enum Orientation {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    MotorStatus motor_status;
    double x;
    double y;
    double v;
    double theta;
    Orientation orientation;
    double omega;
    bool is_moving;
    double dx;
    RobotStatus() {
        motor_status = MotorStatus::STOPPED;
        x = 1.05;
        y = 0.15;
        v = 0;
        theta = PI;
        orientation = UP;
        omega = 0;
        is_moving = false;
        dx = 50;
    }
};

class Localization {
public:
    Localization(Motors& motors, TFLidar& lidar, Flame& flame);
    bool GetPosition();
    bool UpdateLocation();
    bool TurnRobot();
    bool MoveForward();
    MotorStatus GoStraight();
    bool ScanMode();
    bool CenterRobot();
private:
    // Sensors
    TFLidar& lidar_;
    MPU9250_DMP& imu_;
    Flame& flame_;

    // Actuators
    Motors& motors_;
    RobotStatus robot_status_;
};

Localization::Localization(Motors& motors, TFLidar& lidar, Flame& flame) {
    lidar.begin(&Serial3);
}

// in cm
bool Localization::MoveForward(int distance, double tolerance = 1) {
    // need to know our initial distance
    // read Lidar to get distance -> use this to compare to delta
    TFStatus st = lidar.measure();
    while (st != TF_STATUS_OKAY) {
        delay(robot_status_.dx);
        st = lidar.measure();
    }
    // get initial lidar measurement
    uint16_t initial_dist = lidar.getDistance();
    // get initial location
    double init_x = robot_status_.x;
    double init_y = robot_status_.y;
    double init_orientation = robot_status_.orientation;
    // compute final location
    double final_x, final_y;
    switch(robot_status_.orientation) {
        case RobotStatus::Orientation::UP:
            final_y = init_y + distance;
            break;
        case RobotStaus::Orientation::DOWN:
            final_y = init_y - distance;
            break;
        case RobotStatus::Orientation::LEFT:
            final_x = init_x - distance;
            break;
        case RobotStatus::Orientaiton::RIGHT:
            final_y = init_x + distance;
            break;
        case default:
            // wtf??
            break;
    }

    // while not at final location
    while (abs(robot_status_.x - final_x) > LIDAR_TOL || abs(robot_status_.y - final_y) > LIDAR_TOL) {
        MotorStatus motor_status = GoStraight();
        delay(robot_status_.dx);
        UpdateLocation();
    }
    motors_.Halt();
    return true;
}

// going straight with motors and imu
void Localization::GoStraight() {
    if (abs(robot_status_.orientation - init_orientation) <= GYRO_TOL
        && robot_status_.motor_status != MotorStatus::FORWARD) {
        motors_.GoStraight();
        robot_status_.motor_status = MotorStatus::STRAIGHT;
    } else if (robot_status_.orientation - init_orientation > GYRO_TOL
               && robot_status_.motor_status != MotorStatus::SLIGHT_RIGHT) {
        motors_.SlightRight();
        robot_status_.motor_status = MotorStatus::SLIGHT_RIGHT;
    } else if (robot_status_.orientation - init_orientation > GYRO_TOL
               && robot_status_.motor_status != MotorStatus::SLIGHT_LEFT) {
        motors_.SlightLeft();
        robot_status_.motor_status = MotorStatus::SLIGHT_LEFT;
    }
    // know nominal values (motor values and orientation)
    // if orientation is within tolerance && !STRAIGHT || !_isMoving
    // go moveForward
    // if currentOrientation < initialOrientation && !SLIGHT_RIGHT
    // slight increase in right motors
    // if currentOrientation > initialOrientation && !SLIGHT_LEFT
    // slight increase in left motors
}

std::vector<std::pair<int,int>> Localization::ScanMode() {
    motors_.TurnLeft();
    imu_.computeEulerAngles(true);
    float final_yaw = imu_.yaw;
    int revolutions = 0;

    while (abs(final_yaw - robot_status_.orientation) > GYRO_TOL && revolutions < 1) {
        uint64_t lidar_reading = lidar_.getDistance();
        // calculate where the lidar reading is

        FlameLocation flame_location = flame_.locate();

        if (flame_location == FLAME_IS_CENTERED) {
            motors_.Halt();
            fan_.Start();
            while (flame);
            fan_.Stop();
            motors_.TurnLeft();
        }
    }

    // turn left
    // while orientation is not reached
    //   get lidar point
    //   if lidar point is not the wall
    //     check flame sensors
    //     if flame sensor detects fire
    //       stop
    //       while flame still there
    //         turn on fan
    //       turn off fan
    //       turn left
    //     else
    //       add grid coordinate
    // stop
    // return grid coordinates
}
# endif // LOCALIZATION_H