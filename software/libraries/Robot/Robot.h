#ifndef ROBOT_H
#define ROBOT_H

#include <stdio.h> // Not sure if this is needed.
#include <Stack.h>

const int MAP_WIDTH = 6;
const int MAP_HEIGHT = 6;

const int GOAL_CAP = 35;
const int POI_CAP = 35;

const int START_X = 3;
const int START_Y = 5;

class Robot {
  public:
    Robot();
    void go();

  private:
    enum State {
        PATH_PLAN_SURVEY_A,
        PATH_PLAN_SURVEY_B,
        PATH_PLAN,
        HOUSE,
        STRAIGHT,
        TURN_RIGHT,
        TURN_LEFT,
        DONE
    };

    enum Tile {
        UNKNOWN,
        FLAT,
        WATER,
        SAND,
        MISSION,
        GRAVEL,
    };

    enum House {
        RED_HOUSE,
        YELLOW_HOUSE
    };

    // Adjust to actual values.
    enum Speed {
        APPROACHING_HOUSE_SPEED = 10,
        REVERSE_APPROACHING_HOUSE_SPEED = -10,
        LEFT_TURN_SPEED = 20,
        RIGHT_TURN_SPEED = 30,
        NORMAL_SPEED = 40,
    };

    Tile grid[MAP_WIDTH][MAP_HEIGHT];

    Stack<int> pos;
    State st;
    State bufSt;
    State prevSt;

    Stack<Stack<int>> goals;
    Stack<Stack<int>> psoi;

    Speed speed;

    int initialDistFromStopPos;
    int distTravelled;
    int targetDist;

    int angleTravelled;
    int targetAngle;

    // Initialization
    void initializeGrid();
    void initializeFlame();
    void initializeUltrasonic();
    void initializeLidar();
    void initializeGravity();
    void initializeColor();
    void initializeIMU();
    void initializeMotors();
    void initializeLED();

    // State functions
    void pathPlanSurveyAState();
    void pathPlanSurveyBState();
    void pathPlanState();
    void straightState();
    void houseState();
    void turnLeftState();
    void turnRightState();

    // Routing
    void locatePOI();
    void turnToNextGoal();
    void removeGoal();
    void removePOI();
    void computeNextPOIGoal();
    void setPos(int x, int y);
    bool isAtGoal();
    bool isAtLastGoal();
    bool isFacingNextGoal();
    Stack<Stack<int>> planPath(Stack<int> b, Stack<int> e);

    // Missions
    House identifyHouse();
    void inidicateRedHouse();
    void indiciateYellowHouse();

    // Movement
    void halt();
    void drive();
    void turnLeft();
    void turnRight();
    void setSpeed();
    void setInitialDistance();

    // Grid
    Tile readGrid(int x, int y);
    void writeGrid(int x, int y, Tile t);

    // Localization
    void localize();
};

#endif
