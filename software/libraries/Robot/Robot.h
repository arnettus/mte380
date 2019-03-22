#ifndef ROBOT_H
#define ROBOT_H

#include <stdio.h> // Not sure if this is needed.
#include <StackArray.h>
#include <QueueArray.h>
#include <Flame.h>
#include <Ultrasonic.h>
#include <Colour.h>
#include <Gravity.h>
#include <Beacon.h>
#include <Navigator.h>
#include <Fan.h>
#include <Wire.h>

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

const int FIRE_FIGHTING_TIME = 2000;
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
    Robot(int leftFlamePin, int rightFlamePin,
    int leftTrigPinUltra, int leftEchoPinUltra,
    int rightTrigPinUltra, int rightEchoPinUltra,
    int S0, int S1, int S2, int S3, int OUT,  int capacity,
    int redPin, int greenPin, int bluePin
    );

    void pathPlan(StackArray<Coordinate> *path, Coordinate e);

    enum Tile {
        FLAT,
        WATER,
        SAND,
        GRAVEL,
        MISSION,
        CANDLE,
    };

    Tile grid[MAP_WIDTH][MAP_HEIGHT] = {
        {FLAT, FLAT, FLAT, WATER , FLAT, FLAT},
        {FLAT, GRAVEL, FLAT, FLAT, SAND, FLAT},
        {WATER ,FLAT, FLAT, FLAT, FLAT, FLAT},
        {FLAT, FLAT, FLAT, SAND, FLAT, GRAVEL},
        {FLAT, SAND, FLAT, FLAT, WATER , FLAT},
        {FLAT, FLAT, GRAVEL, FLAT, FLAT, FLAT}
    };

    void initializeSensors();
    void go();

  private:

    void printState();
    void printGoals();

    int numTimesCalledLocatePoi;
    void Robot::pathPlanSurveyAStateSpecial();

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
    Coordinate pos;
    Direction dir;
    State st;
    State bufSt;
    State prevSt;

    StackArray<Coordinate> goals;
    StackArray<Coordinate> psoi;
    StackArray<Coordinate> housePsoi;
    StackArray<Coordinate> foodpsoi;

    Flame leftFlame;
    Flame rightFlame;
    Ultrasonic leftUltra;
    Ultrasonic rightUltra;
    Colour colo;
    Gravity grav;

    Beacon beac;
    Navigator nav;
    Fan fan;

    bool foodFound;

    int targetDistToGoal;
    int housesVisited;

    int cLEDPin;
    int redHouseLed;
    int yellowHouseLed;

    bool surveyBEnabled;

    // Initialization
    void detectFood();

    // navigator placeholders ->
    void haltNav();
    void navGoForward(int dist);
    void navGoReverse(int dist);
    void navTurnLeft();
    void navTurnRight();

    // State functions
    void pathPlanState();
    void straightState();
    void houseState();
    void turnLeftState();
    void turnRightState();

    // Path planning
    int heuristic(Coordinate a, Coordinate b);

    // Goals
    void turnTowardsNextGoal();
    void computeNextSurveyAGoal();
    bool isAtGoal();
    bool isAtLastGoal();
    bool changedStateToTurnTowardsCoordinate(Coordinate c);

    // Points of Interest (only relevant during survey mode really)
    void locatePOI();
    void computeNextPOIGoal();

    // Missions
    bool isFireAlive;
    void putOutFire();
    Colour::ColourType identifyHouse();
    void inidicateRedHouse();
    void indiciateYellowHouse();

    // Grid
    bool isOnRow(int y);

    // Sensors
    void updateCurrentPosition();
    void emptyGoals();

    void neighbours(StackArray<Coordinate> *n, int x, int y);

    // Distances
    int turnCost(Coordinate currParent, Coordinate curr, Coordinate nxt);
    Direction dirFromParent(Coordinate parent, Coordinate current);
    int tileCost(Tile t);
};

#endif
