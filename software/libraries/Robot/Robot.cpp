#include "Robot.h"

Robot::Robot(
        lidarCapacity,
        leftFlamePin,
        rightFlamePin,
        fanPin,
        rightSonicTrigPin,
        rightSonicEchoPin,
        leftSonicTrigPin,
        leftSonicEchoPin,
        cS0,
        cS1,
        cS2,
        cS3,
        cOUT,
        cLEDPin,
        redHouseLed,
        yellowHouseLed
    ) :
    st(PATH_PLAN_SURVEY_A),
    prevSt(STRAIGHT),
    goals(GOAL_CAP),
    psoi(POI_CAP),
    pos(START_X, START_Y),
    lidar(lidarCapacity),
    leftFlame(leftFlamePin),
    rightFlame(rightFlamePin),
    fan(fanPin),
    rightSonic(rightSonicTrigPin, rightSonicEchoPin),
    leftSonic(leftSonicTrigPin, leftSonicEchoPin),
    colourSensor(cS0, cS1, cS2, cS3, cOUT),
    cLEDPin(cLEDPin),
    redHouseLed(redHouseLed),
    yellowHouseLed(yellowHouseLed)

{
    initializeGrid();

    goals.push(pos);
    targetAngle = STANDARD_TARGET_ANGLE;

    // flags
    missionCompleted = false;
    checkedForObjectInFront = false;
    waterDetected = false;
    reverse = false;
}

void Robot::initializeSensors() {
    initializeFireFighter();
    initializeLidar();
    initializeGravity();
    initializeColour();
    initializeNav();
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

void Robot::initializeGravity() {
    gravities.Init();
}

void Robot::initializeNav() {
    if (!nav.begin()) {
        Serial.println("Navigator failed to begin");
        while (1) {
            delay(1000);
        }
    }
}

void Robot::initializeColour() {
    pinMode(cLedPin, OUTPUT);
    digitalWrite(cLedPin, HIGH); // maybe you can just turn this on right before you read?
}

void Robot::pathPlanSurveyAState() {
    nav.halt();

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
        detectTiles();

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
    nav.halt();

    if(prevSt == STRAIGHT && isAtLastGoal()) {
        st = HOUSE;
        removePOI();
    } else {
        if(isAtGoal()) removeGoal();
        // maybe do it here, re-compute goals???

        if(!changedStateToTurnTowardsNextGoal()) {
            setTargetDistToGoal();
            st = STRAIGHT;
        }
    }

    prevSt = PATH_PLAN;
}

void Robot::houseState() {
    if(prevSt == PATH_PLAN){
        targetDistToGoal = HOUSE_PROXIMITY;
        st = STRAIGHT;
    } else if(prevSt == STRAIGHT && !missionCompleted) {
        nav.halt();

        House h = identifyHouse(); // maybe an average reading here???
        h == RED ? inidicateRedHouse() : indiciateYellowHouse();

        missionCompleted = true;
        reverse = true;
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

        if(reverse): nav.goReverse();
        else: nav.goForward();
    } else {
        reverse = false; // default is go straight.

        updateCurrentPosition(); // updates waterDetected

        // Consider, every time you stop, to empty your goals and re-compute path
        if(distTravelled >= targetDistToGoal || waterDetected) { // could use isAtGoal check instead?
            waterDetected = false; // in case water is detected but you're already at your goal anyway
            st = bufSt;
        } else if(waterDetected) { // only updates after position is updated
            waterDetected = false;
            emptyGoals();
            computeNextPOIGoal();
            st = bufSt;
        }
    }

    prevSt = STRAIGHT;
}

void Robot::turnLeftState() {
    nav.turnLeft();

    st = prevSt;
    prevSt = TURN_LEFT;
}

void Robot::turnRightState() {
    nav.turnRight();

    st = prevSt;
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
    Direction ori = nav.getCurrentDirection();

    if(nextGoal.x > pos.x) { // go east
        if(ori != East) {
            if(st == NORTH) st = TURN_RIGHT;
            else st = TURN_LEFT;

            turned = true;
        }
    } else if(nextGoal.x < pos.x) { // go west
        if(ori != West) {
            if(st == South) st = TURN_RIGHT;
            else st = TURN_LEFT;

            turned = true;
        }
    } else if(nextGoal.y > pos.y) { // go north
        if(ori != North) {
            if(st == WEST) st = TURN_RIGHT;
            else st = TURN_LEFT;

            turned = true;
        }
    } else { // go south
        if(ori != South) {
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
    Direction ori = nav.getCurrentDirection();

    if(numTilesAdvanced > 0) {
        switch (ori) {
            case North:
                pos.y -= numTilesAdvanced;
            case South:
                pos.y += numTilesAdvanced;
            case West:
                pos.x -= numTilesAdvanced;
            case East:
                pos.x += numTilesAdvanced;
        }

        detectTiles();
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

    if(dist <= 30) return gravities.GetDistance(VERTICAL);
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

void Robot::detectTiles() {
    Direction ori = nav.getCurrentDirection();

    switch (ori) {
        case North:
            if(pos.x != 0): grid[pos.y][pos.x-1] = gravities.GetTerrainType(LEFT);
            if(pos.x != 5): grid[pos.y][pos.x+1] = gravities.GetTerrainType(RIGHT);
            if(pos.y != 0) {
                grid[pos.y-1][pos.x] = gravities.GetTerrainType(VERTICAL);
                if(grid[pos.y-1][pos.x] == WATER): waterDetected = true;
            }
        case South:
            if(pos.x != 0): grid[pos.y][pos.x-1] = gravities.GetTerrainType(RIGHT);
            if(pos.x != 5): grid[pos.y][pos.x+1] = gravities.GetTerrainType(LEFT);
            if(pos.y != 5) {
                grid[pos.y+1][pos.x] = gravities.GetTerrainType(VERTICAL);
                if(grid[pos.y+1][pos.x] == WATER): waterDetected = true;
            }
        case East:
            if(pos.y != 0): grid[pos.y-1][pos.x] = gravities.GetTerrainType(LEFT);
            if(pos.y != 5): grid[pos.y+1][pos.x] = gravities.GetTerrainType(RIGHT);
            if(pos.x != 5) {
                grid[pos.y][pos.x+1] = gravities.GetTerrainType(VERTICAL);
                if(grid[pos.y][pos.x+1] == WATER): waterDetected = true;
            }
        case West:
            if(pos.y != 0): grid[pos.y-1][pos.x] = gravities.GetTerrainType(RIGHT);
            if(pos.y != 5): grid[pos.y+1][pos.x] = gravities.GetTerrainType(LEFT);
            if(pos.x != 0) {
                grid[pos.y][pos.x-1] = gravities.GetTerrainType(VERTICAL);
                if(grid[pos.y][pos.x-1] == WATER): waterDetected = true;
            }
    }
}

ColourType Robot::identifyHouse() {
    // we may want to take an average reading here
   return colourSensor.ReadColour();
}

void Robot::inidicateRedHouse() {
    digitalWrite(redHouseLed, HIGH);
}

void Robot::inidicateYellowHouse() {
    digitalWrite(yellowHouseLed, HIGH);
}

// Wait to finish path planning for these two:
// Tells you to move to the row above you.
void Robot::computeNextSurveyAGoal() {

}

// The actual path planning entry function.
void Robot::computeNextPOIGoal() {

}

// Hope that we never have to actually implement this.
void Robot::pathPlanSurveyBState() {

}
