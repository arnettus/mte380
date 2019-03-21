#include "Robot.h"

Robot::Robot(
        int lidarCapacity,
        int leftFlamePin,
        int rightFlamePin,
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
    ) :
    st(PATH_PLAN_SURVEY_A),
    prevSt(STRAIGHT),
    pos{START_X, START_Y},
    lidar(lidarCapacity),
    flameLeft(leftFlamePin),
    flameRight(rightFlamePin),
    rightSonic(rightSonicTrigPin, rightSonicEchoPin),
    leftSonic(leftSonicTrigPin, leftSonicEchoPin),
    colourSensor(cS0, cS1, cS2, cS3, cOUT),
    cLEDPin(cLEDPin),
    redHouseLed(redHouseLed),
    yellowHouseLed(yellowHouseLed)
{
    goals.push(pos);

    // flags
    missionCompleted = false;

    // do this every time you do a suveyModeShift, record and pathplan again if so
    checkedForObjectInFront = false;

    reverse = false;
}

void Robot::initializeSensors() {
    initializeFireFighter();
    initializeLidar();
    initializeColour();
    initializeNavigator();
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

void Robot::initializeLidar() {}

void Robot::initializeFireFighter() {}

void Robot::shutDownFireFighter() {}

void Robot::initializeColour() {
    pinMode(cLEDPin, OUTPUT);
    digitalWrite(cLEDPin, HIGH); // maybe you can just turn this on right before you read?
}

void Robot::initializeNavigator() {}

void Robot::haltNav() {}

Direction Robot::navGetCurrentDirection() {}

void Robot::pathPlanSurveyAState() {
    haltNav();
    updateCurrentPosition(); // be cautious where you put this

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
    } else {
        if(isAtGoal()) removeGoal();

        if(prevSt == TURN_LEFT || prevSt == TURN_RIGHT){
            locatePOI();
            emptyGoals();
            computeNextSurveyAGoal(); // your map might have something new now.
        }

        if(!changedStateToTurnTowardsNextGoal()) {
            setTargetDistToGoal();
            st = STRAIGHT;
        }
    }

    prevSt = PATH_PLAN_SURVEY_A;
}

void Robot::pathPlanState() {
    haltNav();

    if(prevSt == STRAIGHT && isAtLastGoal()) {
        removeGoal();
        removePOI();

        st = HOUSE;
    } else {
        removeGoal();

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
        haltNav();

        Colour::ColourType c = identifyHouse();
        c == Colour::RED ? inidicateRedHouse() : indiciateYellowHouse();

        missionCompleted = true;
        reverse = true;
        targetDistToGoal = HOUSE_PROXIMITY;
        st = STRAIGHT;
    } else {
        reverse = false;
        missionCompleted = false;
        computeNextPOIGoal();
        st = PATH_PLAN;
    }

    prevSt = HOUSE;
}

void Robot::straightState() {
    if(reverse) navGoReverse();
    else navGoForward();

    st = prevSt;
    prevSt = STRAIGHT;
    updateCurrentPosition();
}

void Robot::turnLeftState() {
    navTurnLeft();

    st = prevSt;
    prevSt = TURN_LEFT;
}

void Robot::turnRightState() {
    navTurnRight();

    st = prevSt;
    prevSt = TURN_RIGHT;
}

void Robot::emptyGoals() {
    goals.isEmpty();
}

void Robot::removeGoal() {
    goals.pop();
}

void Robot::removePOI() {
    psoi.pop();
}

bool Robot::isAtGoal() {
    Coordinate goal = Coordinate{goals.peek()};
    return goal.x == pos.x && goal.y == pos.y;
}

bool Robot::isAtLastGoal() {
    return isAtGoal() && goals.count() == 1;
}

bool Robot::isOnRow(int y) {
    return pos.y == y;
}

bool Robot::changedStateToTurnTowardsNextGoal() {
    Coordinate nextGoal{goals.peek()};
    bool turned = false;
    Direction ori = navGetCurrentDirection();

    if(nextGoal.x > pos.x) {
        if(ori != East) {
            if(ori == North) st = TURN_RIGHT;
            else st = TURN_LEFT;

            turned = true;
        }
    } else if(nextGoal.x < pos.x) {
        if(ori != West) {
            if(ori == South) st = TURN_RIGHT;
            else st = TURN_LEFT;

            turned = true;
        }
    } else if(nextGoal.y > pos.y) {
        if(ori != North) {
            if(ori == West) st = TURN_RIGHT;
            else st = TURN_LEFT;

            turned = true;
        }
    } else {
        if(ori != South) {
            if(ori == East) st = TURN_RIGHT;
            else st = TURN_LEFT;

            turned = true;
        }
    }

    return turned;
}

void Robot::updateCurrentPosition() {
    pos.x = goals.peek().x;
    pos.y = goals.peek().y;
}

void Robot::locatePOI() {
    if(!checkedForObjectInFront) {
        checkedForObjectInFront = true;
        int distFront = distanceInFront();

        if(distFront + FRONT_TOL < expectedDistanceInFront()) {
            int yTilesAway = numTilesAway(distFront);
            grid[pos.y-yTilesAway][pos.x] = MISSION;
        }
    }

    long leftSonicReading = leftSonic.ReadAverageDistance(100);
    long rightSonicReading = rightSonic.ReadAverageDistance(100);

    if(leftSonicReading + SONIC_TOL < expectedDistanceOnLeft()) {
        int poiX = pos.x - numTilesAway(leftSonicReading);
        int poiY = pos.y;

        Coordinate c = Coordinate{poiX, poiY};
        grid[poiY][poiX] = MISSION;
        psoi.push(c);

        if(flameLeft.isFlameInSight()) {
            navTurnLeft();
            putOutFire();
            navTurnRight();

            // The poi was a candle.
            grid[poiY][poiX] = CANDLE;
            psoi.pop();
        }
    }

    if(rightSonicReading + SONIC_TOL < expectedDistanceOnRight()) {
        int poiX = pos.x + numTilesAway(rightSonicReading);
        int poiY = pos.y;

        Coordinate c{poiX, poiY};
        grid[poiY][poiX] = MISSION;
        psoi.push(c);

        if(flameRight.isFlameInSight()) {
            navTurnRight();
            putOutFire();
            navTurnLeft();

            // The poi was a candle.
            grid[poiY][poiX] = CANDLE;
            psoi.pop();
        }
    }
}

int Robot::distanceInFront() {
    int dist = lidar.getDistance();
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

int Robot::numTilesAway(int distance) {
    return distance/30 + 1;
}

void Robot::putOutFire() {
    // fan.TurnOn();
    delay(FIRE_FIGHTING_TIME);

    // fan.TurnOff();
    // fan.Shutdown();

    isFireAlive = false;
}

Colour::ColourType Robot::identifyHouse() {
    // we may want to take an average reading here
   return colourSensor.ReadColour();
}

void Robot::inidicateRedHouse() {
    digitalWrite(redHouseLed, HIGH);
}

void Robot::indiciateYellowHouse() {
    digitalWrite(yellowHouseLed, HIGH);
}

void Robot::computeNextSurveyAGoal() {
    Coordinate e = findValidSurveyGoal(Coordinate{pos.x, pos.y-1});
    goals = pathPlan(e);
}

Coordinate Robot::findValidSurveyGoal(Coordinate oneAbove) {
    Coordinate validTile;

    if(grid[oneAbove.y][oneAbove.x] == FLAT) {
        validTile = oneAbove;
    } else {
        bool validTileFound = false;
        Coordinate leftOfAbove{oneAbove.x-1, oneAbove.y};
        Coordinate rightOfAbove{oneAbove.x+1, oneAbove.y};

        while(!validTileFound) {
            Tile tileToLeft = grid[leftOfAbove.y][leftOfAbove.x];

            if(tileToLeft == FLAT) {
                validTile = leftOfAbove;
                validTileFound = true;
            } else {
                Tile tileToRight = grid[rightOfAbove.y][rightOfAbove.x];

                if(tileToRight == FLAT) {
                    validTile = rightOfAbove;
                    validTileFound = true;
                } else {
                    leftOfAbove = Coordinate{leftOfAbove.x-1, leftOfAbove.y};
                    rightOfAbove = Coordinate{leftOfAbove.x-1, leftOfAbove.y};
                }
            }
        }
    }

    return validTile;
}

void Robot::computeNextPOIGoal() {
    goals = pathPlan(psoi.peek());
}

StackArray<Coordinate> Robot::pathPlan(Coordinate e) {
    Node costMap[MAP_WIDTH][MAP_HEIGHT];

    QueueArray<Coordinate> frontier;
    costMap[pos.y][pos.x].seen = true;
    costMap[pos.y][pos.x].cost = 0;
    costMap[pos.y][pos.x].parent = Coordinate{pos.x, pos.y};

    frontier.push(pos);

    while(!frontier.isEmpty()) {
        Coordinate current = frontier.pop();

        // get valid neighbours
        StackArray<Coordinate> neighbours;

        int currX = current.x;
        int currY = current.y;

        if(currX == e.x && currY == e.y) {
            break;
        }

        if(currY > 0) { // above
            if(!(grid[currY-1][currX]) == WATER)
                neighbours.push(Coordinate{currX, currY-1});
        }
        if(currY < MAP_HEIGHT-1) { // below
            if(!(grid[currY+1][currX]) == WATER)
                neighbours.push(Coordinate{currX, currY+1});
        }
        if(currX > 0) { // left
            if(!(grid[currY][currX-1]) == WATER)
                neighbours.push(Coordinate{currX-1, currY});
        }
        if(currX < MAP_WIDTH-1) { // right
            if(!(grid[currY][currX+1]) == WATER)
                neighbours.push(Coordinate{currX+1, currY});
        }

        // iterate over each neighbour
        while(!neighbours.isEmpty()) {
            Coordinate nxt = neighbours.pop();

            int newCost = 1 +
                costMap[currY][currX].cost +
                turnCost(costMap[current.y][current.x].parent, current, nxt);
                // tileCost(grid[nxt.self.y][nxt.self.x]);

            if(costMap[nxt.y][nxt.x].seen == false || newCost < costMap[nxt.y][nxt.x].cost) {
                costMap[nxt.y][nxt.x].cost = newCost;
                costMap[nxt.y][nxt.x].parent = Coordinate{currX, currY};
                costMap[nxt.y][nxt.x].seen = true;

                frontier.push(nxt);
            }
        }
    }

    // post process here, go backwards in costMap
    Coordinate focus{e.x,e.y};
    Direction focusDir = Nothing;

    StackArray<Coordinate> path;

    // iterate back to your starting position
    while(!(focus.x == pos.x && focus.y == pos.y)){
        Coordinate p = costMap[focus.y][focus.x].parent;

        Direction nextDir = dirFromParent(p, focus);

        if(nextDir != focusDir) path.push(focus);

        focus = p;
        focusDir = nextDir;
    }

    return path;
}

int Robot::turnCost(Coordinate currParent, Coordinate curr, Coordinate nxt) {
    Direction o;

    if(currParent.x == curr.x && currParent.y == curr.y) {
        //orientation = navGetCurrentDirection();
        o = North;
    } else {
        o = dirFromParent(currParent, curr);
    }

    switch (o) {
        case North:
            if(nxt.y < curr.y) {
                return 0;
            }
            break;
        case South:
            if(nxt.y > curr.y) {
                return 0;
            }
            break;
        case East:
            if(nxt.x > curr.x) {
                return 0;
            }
            break;
        case West:
            if(nxt.x < curr.x) {
                return 0;
            }
            break;
        default:
            break;
    }

    int multiplier = 1;

    if(grid[curr.y][curr.x] == SAND){
        multiplier = 100;
    }

    return 1*multiplier;
}

Direction Robot::dirFromParent(Coordinate parent, Coordinate current) {
    if(parent.y > current.y) return North;
    if(parent.y < current.y) return South;
    if(parent.x > current.x) return West;
    if(parent.x < current.x) return East;
}

int Robot::tileCost(Tile t) {
    switch (t) {
        case SAND:
            return 2;
            break;
        case GRAVEL:
            return 3;
            break;
        default:
            break;
    }

    return 1;
}

void Robot::setTargetDistToGoal() {}

void Robot::pathPlanSurveyBState() {}

void Robot::navGoReverse() {}

void Robot::navGoForward() {}

void Robot::navTurnLeft() {}

void Robot::navTurnRight() {}
