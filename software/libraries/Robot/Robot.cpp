#include "Robot.h"

Robot::Robot() :
    st(PATH_PLAN_SURVEY_A),
    goals(GOAL_CAP),
    psoi(POI_CAP),
    pos(2)
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

    st = PATH_PLAN_SURVEY_A;
    prevSt = STRAIGHT;
    setPos(START_X, START_Y);
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
    }
}

void Robot::pathPlanSurveyAState() {

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

// Determine whether you're looking at a point of interest
// with your side-view ultrasonics. If so, fill psoi stack.
void Robot::locatePOI() {}

bool Robot::isAtGoal() {
    Stack<int> goal = goals.peek();

    return goal == pos;
}

bool Robot::isAtLastGoal() {
    return isAtGoal() && goals.getSize() == 1;
}
