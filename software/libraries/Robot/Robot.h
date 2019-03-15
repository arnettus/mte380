#ifndef ROBOT_H
#define ROBOT_H

#include <stdio.h> // Not sure if this is needed.
#include <Stack.h>
#include <Coordinate.h>

const int MAP_WIDTH = 6;
const int MAP_HEIGHT = 6;

const int GOAL_CAP = 35;
const int POI_CAP = 35;

const int START_X = 3;
const int START_Y = 5;

const int LAST_ROW = 0;

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
        GRAVEL
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
        NORMAL_SPEED = 40
    };

    enum Orientation {
        NORTH = 0,
        EAST = 90,
        SOUTH = 180,
        WEST = 270
    };

    Tile grid[MAP_WIDTH][MAP_HEIGHT];

    Coordinate pos;
    Orientation ori;
    State st;
    State bufSt;
    State prevSt;

    Stack<Coordinate> goals;
    Stack<Coordinate> psoi;

    Speed speed;

    int initialDistFromStopPos;
    int distTravelled;
    int targetDist;

    int angleTravelled;
    int targetAngle;

    bool surveyBEnabled;

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
    void turnTowardsNextGoal();
    void removeGoal();
    void removePOI();
    void computeNextPOIGoal();
    void computeNextSurveyAGoal();
    void emptyGoals();
    bool isAtGoal();
    bool isAtLastGoal();
    bool isFacingNextGoal();
    Stack<Stack<int>> planPath(Stack<int> b, Stack<int> e);

    // Missions
    bool isFireAlive;
    void checkAndKillFire();
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
    bool isOnRow(int y);

    // Sensors
    void localize();
    void detectAdjTiles();
};

#endif
