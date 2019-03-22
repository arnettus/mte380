#ifndef ROBOT_H
#define ROBOT_H

#include <stdio.h> // Not sure if this is needed.
#include <StackArray.h>
#include <TFLidar.h>
#include <Flame.h>
#include <Ultrasonic.h>
#include <Colour.h>
#include <Gravity.h>
#include <QueueArray.h>

const int MAP_WIDTH = 6;
const int MAP_HEIGHT = 6;

const int TILE_LENGTH = 30;
const int HALF_TILE_LENGTH = 15;
const int TILE_WIDTH = 30;
const int HALF_TILE_WIDTH = 15;

const int GOAL_CAP = 35;
const int POI_CAP = 35;

const int START_X = 3;
const int START_Y = 5;

const int LAST_ROW = 0;

const int HOUSE_PROXIMITY = 5; // cm
const int NEGATIVE_HOUSE_PROXIMITY = 5; // cm
const int STANDARD_TARGET_ANGLE = 90; // deg

const int FIRE_FIGHTING_TIME = 200;
const int SONIC_TOL = 10;
const int FRONT_TOL = 10;

enum Direction {
    North = 0,
    East = 90,
    South = 180,
    West = 270,
    Nothing = 999
};

struct Coordinate{
    int x;
    int y;
};

class Robot {
  public:
    Robot(
        int lidarCapacity,
        int leftFlameSensorPin,
        int rightFlameSensoPin,
        int fanPin,
        int rightSonicTrigPin,
        int rightSonicEchoPin,
        int leftSonicTrigPin,
        int leftSonicEchoPin,
        int cS0,
        int cS1,
        int cS2,
        int cS3,
        int cOUT,
        int cLEDPin,
        int redHouseLed,
        int yellowHouseLed
    );

    StackArray<Coordinate> pathPlan(Coordinate e);

    enum Tile {
        FLAT,
        WATER,
        SAND,
        GRAVEL,
        MISSION,
        CANDLE,
    };

    Tile grid[MAP_WIDTH][MAP_HEIGHT] = {
            {FLAT, FLAT, FLAT, SAND, FLAT, FLAT},
            {FLAT, FLAT, FLAT, FLAT, FLAT, FLAT},
            {FLAT, FLAT, FLAT, FLAT, FLAT, FLAT},
            {FLAT, FLAT, FLAT, FLAT, FLAT, FLAT},
            {FLAT, FLAT, FLAT, FLAT, FLAT, FLAT},
            {FLAT, FLAT, FLAT, FLAT, FLAT, FLAT}
    };

    void initializeSensors();
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

    struct Node {
        Coordinate parent;
        bool seen = false;
        int cost = 0;
    };

    // Sensors
    TFLidar lidar;
    Flame flameLeft;
    Flame flameRight;
    Ultrasonic rightSonic;
    Ultrasonic leftSonic;
    Colour colourSensor;

    Coordinate pos;
    Direction dir;
    State st;
    State bufSt;
    State prevSt;

    StackArray<Coordinate> goals;
    StackArray<Coordinate> psoi;

    int targetDistToGoal;
    int housesVisited;

    int cLEDPin;
    int redHouseLed;
    int yellowHouseLed;

    bool surveyBEnabled;

    // Initialization
    void initializeGrid();
    void initializeFireFighter();
    void shutDownFireFighter();
    void initializeLidar();
    void initializeColour();
    void initializeLED();

    // navigator placeholders ->
    void initializeNavigator();
    void haltNav();
    void navGoForward();
    void navGoReverse();
    void navTurnLeft();
    void navTurnRight();

    // State functions
    void pathPlanSurveyAState();
    void pathPlanSurveyBState();
    void pathPlanState();
    void straightState();
    void houseState();
    void turnLeftState();
    void turnRightState();

    // Path planning

    // Goals
    void turnTowardsNextGoal();
    void computeNextSurveyAGoal();
    bool isAtGoal();
    bool isAtLastGoal();
    bool changedStateToTurnTowardsCoordinate(Coordinate c);

    // Points of Interest (only relevant during survey mode really)
    void locatePOI();
    void computeNextPOIGoal();
    bool checkedForObjectInFront;

    // Missions
    bool isFireAlive;
    void putOutFire();
    Colour::ColourType identifyHouse();
    void inidicateRedHouse();
    void indiciateYellowHouse();

    // Movement ---- IMPLEMENT THESE!
    void setInitialDistFromStopPos();
    void setTargetDistToGoal();

    // Grid
    bool isOnRow(int y);

    // Sensors
    void updateCurrentPosition();
    bool findObjectRight(int *tiles);
    bool findObjectLeft(int *tiles);
    void emptyGoals();

    void neighbours(StackArray<Coordinate> n, int x, int y);

    // Distances
    int numTilesAway(int distance);
    Coordinate findValidSurveyGoal(Coordinate oneAbove);
    int turnCost(Coordinate currParent, Coordinate curr, Coordinate nxt);
    Direction dirFromParent(Coordinate parent, Coordinate current);
    int tileCost(Tile t);
};

#endif
