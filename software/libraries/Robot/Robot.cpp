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

        // sensor polling

        // do flanking sensors like this, have a flag for detectadjtiles
        // that turns on and off (off while turning)

        t = millis();
        tdiff = pt - t;

        if (lidarEnabled && tdiff > POLL_LIDAR_TIME)
            poll lidar

        if (gravityEnabled && tdiff > POLL_GRAVITY_TIME)


        // You don't want to turn tile detection on during a turn.
        // Depending on accuracy, you may not even want this on
        // while going straight.
        if(!(st == TURN_LEFT || st == TURN_RIGHT)) {
            detectAdjTiles();
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
    // assume lidar.measure() is being called
    // on an interrupt in the background for now.
    distTravelled = abs(initialDistFromStopPos - distanceInFront());

    // you just check if the distTravelled / 30 floored is greater than numberOfTilesAdvanced
    // update current coordinate
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

// Check all your flame sensors. Rotate in direction of fire.
void Robot::checkAndKillFire() {
    bool flameOnRight = rightFlame.isFlameInSight();
    bool flameOnLeft = leftFlame.isFlameInSight();

    if(flameOnRight || flameOnLeft){
        if(flameOnRight) turnRight();
        else turnLeft();

        fan.TurnOn();
        delay(FIRE_FIGHTING_TIME);
        fan.TurnOff();
        fan.Shutdown()
        isFireAlive = false;

        if(flameOnRight) turnLeft();
        else turnRight();
    }
}

void Robot::locatePOI() {
    if(!checkedForObjectInFront) {
        checkedForObjectInFront = true;
        int distFront = distanceInFront();

        if(distFront < expectedDistanceInFront()) {
            y = mapDistanceToCoordinate();
            grid[y][x] = MISSION;
        }
    }

}

int Robot::distanceInFront() {
    int dist = lidar.getDistance();

    // Once you have gravities,
    // return gravity.straight distance
    if(dist <= 30) return dist;
    return dist;
}

int Robot::expectedDistanceInFront() {

}

int Robot::mapDistanceToCoordinate() {

}


// Not implemented yet:

// Wait to figure out motors for these four.

// Go at the robot's speed.
void Robot::halt() {}

void Robot::drive() {}

void Robot::turnLeft() {}

void Robot::turnRight() {}

// Wait to figure out gravity for this one.

// Make sure to handle edge cases properly.
void Robot::detectAdjTiles() {}

// Wait to figure out LED and colour sensor for these.
// Map Colour type to House type.
void Robot::identifyHouse() {}

void Robot::inidicateRedHouse() {}

void Robot::inidicateYellowHouse() {}

// Hope that we never have to actually implement this.
void Robot::pathPlanSurveyBState() {}

// Wait to finish path planning for these two:

// Tells you to move to the row above you.
void Robot::computeNextSurveyAGoal() {}

// The actual path planning entry function.
void Robot::computeNextPOIGoal() {}
