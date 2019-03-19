#include "Robot.h"

Robot::Robot(
        lidarCapacity,
        leftFlamePin,
        rightFlamePin,
        fanPin,
        rightSonicTrigPin,
        rightSonicEchoPin,
        leftSonicTrigPin,
        leftSonicEchoPin
    ) :
    st(PATH_PLAN_SURVEY_A),
    prevSt(STRAIGHT),
    goals(GOAL_CAP),
    psoi(POI_CAP),
    pos(START_X, START_Y),
    ori(NORTH),
    lidar(lidarCapacity),
    leftFlame(leftFlamePin),
    rightFlame(rightFlamePin),
    fan(fanPin),
    rightSonic(rightSonicTrigPin, rightSonicEchoPin),
    leftSonic(leftSonicTrigPin, leftSonicEchoPin)
{
    initializeGrid();

    goals.push(pos);
    targetAngle = STANDARD_TARGET_ANGLE;
    missionCompleted = false;
    checkedForObjectInFront = false;
}

void Robot::initializeSensors() {
    initializeMotors();
    initializeFlame();
    initializeFireFighter();
    initializeLidar();
    initializeGravity();
    initializeColor();
    initializeIMU();
    initializeLED();
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
    }
}

void Robot::initializeFireFighter() {
    fan.Setup();
}

void Robot::shutDownFireFighter() {
    fan.Shutdown();
}

void Robot::initializeFan() {
    fan.Setup();
}

void Robot::pathPlanSurveyAState() {
    halt();

    if(goals.isEmpty()) {
        if(isOnRow(LAST_ROW)) {
            if(surveyBEnabled) {
                st = PATH_PLAN_SURVEY_B;
            } else {
                st = PATH_PLAN;
                computeNextPOIGoal();
            }
        } else {
            computeNextSurveyAGoal();
        }
    } else if(prevSt == STRAIGHT && isAtLastGoal()) {
        emptyGoals();
        locatePOI();

        if(isFireAlive) checkAndKillFire();
    } else {
        if(isAtGoal()) removeGoal();

        if(!changedStateToTurnTowardsNextGoal()) {
            setTargetDistToGoal();
            st = STRAIGHT;
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

        if(!changedStateToTurnTowardsNextGoal()) {
            setTargetDistToGoal();
            st = STRAIGHT;
        }
    }

    prevSt = PATH_PLAN;
}

void Robot::houseState() {
    if(prevSt == PATH_PLAN){
        speed = APPROACHING_HOUSE_SPEED;
        targetDistToGoal = HOUSE_PROXIMITY;

        st = STRAIGHT;
    } else if(prevSt == STRAIGHT && !missionCompleted) {
        halt();

        House h = identifyHouse();
        h == RED_HOUSE ? inidicateRedHouse() : indiciateYellowHouse();

        missionCompleted = true;
        speed = REVERSE_APPROACHING_HOUSE_SPEED;
        targetDistToGoal = HOUSE_PROXIMITY;

        st = STRAIGHT;
    } else {
        missionCompleted = false;
        computeNextPOIGoal();

        st = PATH_PLAN;
    }

    prevSt = HOUSE;
}

void Robot::straightState() {
    if(prevSt != STRAIGHT) {
        bufSt = prevSt;

        distTravelled = 0;
        setInitialDistFromStopPos();
        tilesPrevAdvanced = 0;

        drive();
    } else {
        updateCurrentPosition();
        if(distTravelled >= targetDistToGoal) st = bufSt;
    }

    prevSt = STRAIGHT;
}

void Robot::turnLeftState() {
    if(prevSt != TURN_LEFT) {
        bufSt = prevSt;
        angleTravelled = 0;

        turnLeft();
    } else {
        if(angleTravelled >= targetAngle) st = bufSt;
    }

    prevSt = TURN_LEFT;
}

void Robot::turnRightState() {
    if(prevSt != TURN_RIGHT) {
        bufSt = prevSt;
        angleTravelled = 0;

        turnRight();
    } else {
        if(angleTravelled == targetAngle) st = bufSt;
    }

    prevSt = TURN_RIGHT;
}

void Robot::initializeGrid() {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            grid[i][j] = UNKNOWN;
        }
    }
}

void Robot::emptyGoals() {
    goals.empty();
}

void Robot::removeGoal() {
    goals.pop();
}

void Robot::removePOI() {
    psoi.pop();
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

bool Robot::changedStateToTurnTowardsNextGoal() {
    Coordinate nextGoal = goals.peek();
    bool turned = false;

    if(nextGoal.x > pos.x) { // go east
        if(ori != EAST) {
            if(st == NORTH) st = TURN_RIGHT;
            else st = TURN_LEFT;

            turned = true;
        }
    } else if(nextGoal.x < pos.x) { // go west
        if(ori != WEST) {
            if(st == SOUTH) st = TURN_RIGHT;
            else st = TURN_LEFT;

            turned = true;
        }
    } else if(nextGoal.y > pos.y) { // go north
        if(ori != NORTH) {
            if(st == WEST) st = TURN_RIGHT;
            else st = TURN_LEFT;

            turned = true;
        }
    } else { // go south
        if(ori != SOUTH) {
            if(st == EAST) st = TURN_RIGHT;
            else st = TURN_LEFT;

            turned = true;
        }
    }

    return turned;
}

void Robot::updateCurrentPosition() {
    distTravelled = abs(initialDistFromStopPos - distanceInFront());

    int numTilesAdvanced = distTravelled/30 - tilesPrevAdvanced;

    if(numTilesAdvanced > 0) {
        switch (ori) {
            case NORTH:
                pos.y -= numTilesAdvanced;
            case SOUTH:
                pos.y += numTilesAdvanced;
            case WEST:
                pos.x -= numTilesAdvanced;
            case EAST:
                pos.x += numTilesAdvanced;
        }

        // do water detection check here, u dont need it on tight pollll
        // do tile detection here too to be honestttttttt
        // basically only need to do it when you advance a tile, to see, what the one in front is
    }

    tilesPrevAdvanced = numTilesAdvanced;
}

void Robot::locatePOI() {
    if(!checkedForObjectInFront) {
        checkedForObjectInFront = true;
        int distFront = distanceInFront();

        if(distFront < expectedDistanceInFront()) {
            int yTilesAway = numTilesAway(distFront);
            grid[pos.y-yTilesAway][pos.x] = MISSION;
        }
    }

    leftSonicReading = leftSonic.ReadAverageDistance();
    rightSonicReading = rightSonic.ReadAverageDistance();

    if(leftSonicReading < expectedDistanceOnLeft()) {
        int poiX = pos.x - numTilesAway(leftSonicReading);
        int poiY = pos.y;

        Coordinate c = Coordinate(poiX, poiY);
        grid[poiY][poiX] = MISSION;
        psoi.add(c);

        if(leftFlame.isFlameInSight()) {
            turnLeft();
            putOutFire();
            turnRight();

            // The poi was a candle.
            grid[poiY][poiX] = CANDLE;
            psoi.pop();
        }
    }

    if(rightSonicReading < expectedDistanceOnRight()) {
        int poiX = pos.x + numTilesAway(leftSonicReading);
        int poiY = pos.y;

        Coordinate c = Coordinate(poiX, poiY);
        grid[poiY][poiX] = MISSION;
        psoi.add(c);

        if(rightFlame.isFlameInSight()) {
            turnRight();
            putOutFire();
            turnLeft();

            // The poi was a candle.
            grid[poiY][poiX] = CANDLE;
            psoi.pop();
        }
    }
}

int Robot::distanceInFront() {
    int dist = lidar.getDistance();

    if(dist <= 30) return dist; // update to gravity
    return dist;
}

int Robot::expectedDistanceInFront() {
    return pos.y*TILE_LENGTH;
}

int Robot::expectedDistanceOnRight() {
    return (MAP_WIDTH-pos.x-1)*TILE_WIDTH;
}

int Robot::expectedDistanceOnLeft() {
    return (pos.x)*TILE_WIDTH;
}

int Robot::numTilesAway(dist) {
    return dist/2 + 1;
}

void Robot::putOutFire() {
    fan.TurnOn();
    delay(FIRE_FIGHTING_TIME);

    fan.TurnOff();
    fan.Shutdown()

    isFireAlive = false;
}
// Not implemented yet:

// Wait to figure out gravity for this one.
void Robot::detectAdjTiles() {}

// Navigator
void Robot::halt() {}

void Robot::drive() {}

void Robot::turnLeft() {}

void Robot::turnRight() {}

// Colour
void Robot::identifyHouse() {}

void Robot::inidicateRedHouse() {}

void Robot::inidicateYellowHouse() {}

// Wait to finish path planning for these two:
// Tells you to move to the row above you.
void Robot::computeNextSurveyAGoal() {}

// The actual path planning entry function.
void Robot::computeNextPOIGoal() {}

// Hope that we never have to actually implement this.
void Robot::pathPlanSurveyBState() {}
