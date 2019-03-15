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

        // Turn off lidar readings during a turn state?
        localize();

        // You don't want to turn tile detection on during a turn.
        // Depending on accuracy, you may not even want this on
        // while going straight.
        if(!(st == TURN_LEFT || st == TURN_RIGHT)) {
            detectAdjTiles();
        }
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
            st = PATH_PLAN;
            if(surveyBEnabled) st = PATH_PLAN_SURVEY_B;
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
        // Do you even need this check? If you entered this path,
        // it should be assumed that you reached a goal that wasn't
        // your final goal.
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
    if(prevSt == STRAIGHT && isAtLastGoal()) {
        st = HOUSE;
        removePOI();
    } else {
        // Do you even need this check? If you entered this path,
        // it should be assumed that you reached a goal that wasn't
        // your final goal.
        if(isAtGoal()) removeGoal();

        if(isFacingNextGoal()) {
            st = STRAIGHT;
        } else {
            turnTowardsNextGoal();
        }
    }

    prevSt = PATH_PLAN;
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
// next goal's position.
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

void Robot::halt() {}

void Robot::localize() {}

// Make sure to handle edge cases properly.
void detectAdjTiles() {}
