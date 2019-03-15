#include "Robot.h"

Robot::Robot() :
    st(PATH_PLAN_SURVEY_A),
    prevSt(STRAIGHT),
    goals(GOAL_CAP),
    psoi(POI_CAP),
    pos(START_X, START_Y),
    ori(NORTH)
{
    Robot::initializeGrid();

    Robot::initializeMotors();
    Robot::initializeFlame();
    Robot::initializeUltrasonic();
    Robot::initializeLidar();
    Robot::initializeGravity();
    Robot::initializeColor();
    Robot::initializeIMU();
    Robot::initializeLED();

    // Initialize survey mode properly.
    goals.push(pos);
}

void Robot::go() {
    while(st != DONE) {
        switch (st) {
            case PATH_PLAN_SURVEY_A:
                pathPlanSurveyAState();
            case PATH_PLAN_SURVEY_B:
                pathPlanSurveyBState();
            case PATH_PLAN:
                pathPlanState();
            case HOUSE:
                houseState();
            case STRAIGHT:
                straightState();
            case TURN_LEFT:
                turnLeftState();
            case TURN_RIGHT:
                turnRightState();
        }

        // sensor polling

        // You don't want to turn tile detection on during a turn.
        // Depending on accuracy, you may not even want this on
        // while going straight.
        if(!(st == TURN_LEFT || st == TURN_RIGHT)) {
            detectAdjTiles();
        }

        // If detect water, empty goals and go to path planning.
    }
}

// This version of PATH_PLAN_SURVEY_A does not rely on turns
// to find points of interest. Rather, ultrasonics try to cover
// the distance.
void Robot::pathPlanSurveyAState() {
    halt();

    // Differed from blackboard -> you should first check if your goals
    // are empty. Then check if you just stopped at your goal.
    if(goals.isEmpty()) {
        if(isOnRow(LAST_ROW)) {
            if(surveyBEnabled) {
                st = PATH_PLAN_SURVEY_B;
            } else {
                st = PATH_PLAN;

                // Differed from blackboard -> this needs to set up path_plan right?
                computeNextPOIGoal();
            }
        } else {
            computeNextSurveyAGoal();
        }
    } else if(prevSt == STRAIGHT && isAtLastGoal()) {
        // Differed from blackboard -> you only need to locate POI and
        // check for fire when you know you're at your row goal. As
        // opposed to every time you enter this state. The other times
        // you can enter this state are when you need to dodge a water
        // tile in front of you during survey.

        emptyGoals();
        locatePOI();
        if(isFireAlive) checkAndKillFire();
    } else {
        if(isAtGoal()) removeGoal();

        if(isFacingNextGoal()) {
            st = STRAIGHT;
        } else {
            turnTowardsNextGoal();
        }
    }

    prevSt = PATH_PLAN_SURVEY_A;
}

void Robot::pathPlanState() {
    halt();

    if(prevSt == STRAIGHT && isAtLastGoal()) {
        st = HOUSE;
        removePOI();
    } else {
        if(isAtGoal()) removeGoal();

        if(isFacingNextGoal()) {
            st = STRAIGHT;
        } else {
            turnTowardsNextGoal();
        }
    }

    prevSt = PATH_PLAN;
}

void Robot::houseState() {
    if(prevSt == PATH_PLAN){
        speed = APPROACHING_HOUSE_SPEED;
        targetDistToGoal = HOUSE_PROXIMITY;
        st = STRAIGHT;
    } else if(prevSt == STRAIGHT) {
        halt();

        House h = identifyHouse();
        h == RED_HOUSE ? inidicateRedHouse() : indiciateYellowHouse();

        speed = REVERSE_APPROACHING_HOUSE_SPEED;
        targetDistToGoal = HOUSE_PROXIMITY;
        st = STRAIGHT;
    } else if(prevSt == HOUSE) {
        computeNextPOIGoal();
        st = PATH_PLAN;
    }

    prevSt = HOUSE;
}

void Robot::straightState() {
    if(prevSt != STRAIGHT) {
        // Store previous state so you can return
        // to it when you hit your target distance.
        bufSt = prevSt;

        // initialDistFromStopPos is based on lidar reading
        // from stop state. targetDistToGoal is based on
        // distance to location of goal on grid.
        distTravelled = 0;
        setTargetDistToGoal();
        setInitialDistFromStopPos();

        drive();
    } else {
        localize(); // Do we only want to call this while moving straight?
        if(distTravelled == targetDistToGoal) st = bufSt;
    }

    prevSt = STRAIGHT;
}

void Robot::turnLeftState() {
    if(prevSt != TURN_LEFT) {
        bufSt = prevSt;
        angleTravelled = 0;

        turnLeft();
    } else {
        // Differed from blackboard -> commented localize out, we only want to do
        // imu localization right???
        // localize();

        if(angleTravelled == targetAngle) st = bufSt;
    }

    prevSt = TURN_LEFT;
}

void Robot::turnRightState() {
    if(prevSt != TURN_RIGHT) {
        bufSt = prevSt;
        angleTravelled = 0;

        turnRight();
    } else {
        // Differed from blackboard -> commented localize out, we only want to do
        // imu localization right???
        // localize();

        if(angleTravelled == targetAngle) st = bufSt;
    }

    prevSt = TURN_RIGHT;
}

void Robot::initializeGrid() {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_HEIGHT; ++j) {
            grid[i][j] = UNKNOWN;
        }
    }
}

Robot::Tile Robot::readGrid(int x, int y) {
    return grid[y][x];
}

void Robot::writeGrid(int x, int y, Tile t) {
    grid[y][x] = t;
}

void Robot::emptyGoals() {
    goals.empty();
}

void Robot::removeGoal() {
    goals.pop();
}

bool Robot::isAtGoal() {
    Coordinate goal = goals.peek();

    return goal == pos;
}

bool Robot::isAtLastGoal() {
    return isAtGoal() && goals.getSize() == 1;
}

bool Robot::isOnRow(int y) {
    return pos.y == y;
}

// Not implemented yet:

// Compare Robot's current orientation and position to
// next goal's position.
bool Robot::isFacingNextGoal() {}

// Change state to TURN_LEFT or TURN_RIGHT depending on
// next goal's position. Set targetAngle here!
void Robot::turnTowardsNextGoal() {}

// Hope that we never have to actually implement this.
void Robot::pathPlanSurveyBState() {}

// Determine whether you're looking at a point of interest
// with your side-view ultrasonics. If so, fill psoi stack.
void Robot::locatePOI() {}

// Check all your flame sensors. Rotate in direction of fire.
// Turn on fan.
void Robot::checkAndKillFire() {}

// Tells you to move to the row above you.
void Robot::computeNextSurveyAGoal() {}

// The actual path planning entry function.
void Robot::computeNextPOIGoal() {}

void Robot::halt() {}

void Robot::localize() {}

// Make sure to handle edge cases properly.
void detectAdjTiles() {}

// Map Colour type to House type.
void identifyHouse() {}

void inidicateRedHouse() {}

void inidicateYellowHouse() {}

// Go at the robot's speed.
void drive() {}

void turnLeft() {}

void turnRight() {}
