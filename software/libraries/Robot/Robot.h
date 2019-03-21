#ifndef ROBOT_H
#define ROBOT_H

#include <stdio.h> // Not sure if this is needed.
#include <Stack.h>
#include <Coordinate.h>
#include <TFLidar.h>
#include <Flame.h>
#include <Motors.h>
#include <Fan.h>
#include <Ultrasonic.h>
#include <Colour.h>
#include <Navigator.h>
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

    // Adjust based on terrain class
    enum Tile {
        UNKNOWN,
        FLAT,
        WATER,
        SAND,
        GRAVEL,
        MISSION,
        CANDLE,
    };

    struct Node {
        Coordinate self;
        Coordinate parent;
        bool seen = false;
        int cost = 0;
        int priority = 0;
    };

    // Sensors
    TFLidar lidar;
    flameLeft Flame;
    flameRight Flame;
    fan Fan;
    rightSonic Ultrasonic;
    leftSonic Ultrasonic;
    GravitySensors gravities;
    colourSensor Colour;
    nav Navigator;

    Tile grid[MAP_WIDTH][MAP_HEIGHT];

    Coordinate pos;
    State st;
    State bufSt;
    State prevSt;

    Stack<Coordinate> goals;
    Stack<Coordinate> psoi;

    int initialDistFromStopPos;
    int tilesPrevAdvanced;
    int distTravelled;
    int targetDistToGoal;

    int cLEDPin;
    int redHouseLed;
    int yellowHouseLed;

    int angleTravelled;
    int targetAngle;

    bool surveyBEnabled;
    bool missionCompleted;
    bool reverse;

    // Initialization
    void initializeGrid();
    void initializeFireFighter();
    void initializeLidar();
    void initializeGravity();
    void initializeColour();
    void initializeIMU();
    void initializeLED();

    // State functions
    void pathPlanSurveyAState();
    void pathPlanSurveyBState();
    void pathPlanState();
    void straightState();
    void houseState();
    void turnLeftState();
    void turnRightState();

    // Path planning
    Stack<Coordinate> planPath(Coordinate b, Coordinate e);

    // Goals
    void turnTowardsNextGoal();
    void removeGoal();
    void removePOI();
    void computeNextSurveyAGoal();
    void emptyGoals();
    bool isAtGoal();
    bool isAtLastGoal();
    bool changedStateToTurnTowardsNextGoal();

    // Points of Interest (only relevant during survey mode really)
    void locatePOI();
    void computeNextPOIGoal();
    bool checkedForObjectInFront;

    // Missions
    bool isFireAlive;
    void putOutFire();
    ColourType identifyHouse();
    void inidicateRedHouse();
    void indiciateYellowHouse();

    // Movement
    void setInitialDistFromStopPos();
    void setTargetDistToGoal();

    // Grid
    bool isOnRow(int y);

    // Sensors
    void updateCurrentPosition();
    void detectAdjTiles();
    int distanceInFront();
    int expectedDistanceInFront();
    bool waterDetected;

    // Distances
    int numTilesAway();
};

#endif
