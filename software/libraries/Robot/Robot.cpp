#include "Robot.h"
#include <SoftwareSerial.h>

Robot::Robot() {
}

void Robot::initializeSensors() {
    initializeFireFighter();
    initializeLidar();
    initializeColour();
    initializeNavigator();
}

void Robot::go() {
    // special initial case
    // int tiles;
    // bool objectThere = findObjectRight(&tiles); // on right side

    // if(objectThere) {
    //     grid[pos.y-tiles][pos.x] = MISSION;
    //     psoi.push(Coordinate{pos.x, pos.y-tiles});

    //     if(flameRight.isFlameInSight()) {
    //         navTurnRight();
    //         putOutFire();

    //         // The poi was a candle.
    //         grid[pos.y-tiles][pos.x] = CANDLE;
    //         psoi.pop();
    //     }
    // }

    // if(!(dir == North)) {
    //     navTurnRight();
    // }

    st = PATH_PLAN;
    prevSt = STRAIGHT;
    pos = Coordinate{START_X, START_Y};

    psoi.push(Coordinate{1, 4});
    housesVisited == 0;

    computeNextPOIGoal();
    dir = North;

    while(st != DONE) {
        printState();
        printGoals();

        if(st == PATH_PLAN_SURVEY_A) {
            pathPlanSurveyAState();
        } else if(st == PATH_PLAN) {
            pathPlanState();
        } else if (st == HOUSE) {
            houseState();
        } else if (st == STRAIGHT) {
            straightState();
        } else if (st == TURN_LEFT) {
            turnLeftState();
        } else if (st == TURN_RIGHT) {
            turnRightState();
        }
    }


    Serial.println("you got out!!!!");
    printState();
}

void Robot::printState() {
        Serial.println("");
        Serial.println("-------- STATE ---------");
        Serial.print(" st: ");
        Serial.println(st);
        Serial.print(" pos: ");
        Serial.print(pos.x);
        Serial.print(",");
        Serial.println(pos.y);
        Serial.print(" prevSt: ");
        Serial.print(prevSt);
        Serial.println("");
}

void Robot::initializeLidar() {}

void Robot::initializeFireFighter() {}

void Robot::shutDownFireFighter() {}

void Robot::initializeColour() {}

void Robot::initializeNavigator() {}

void Robot::haltNav() {
    return;
}

void Robot::pathPlanSurveyAState() {
    haltNav();

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
        if(isAtGoal()) goals.pop();

        if(prevSt == TURN_LEFT || prevSt == TURN_RIGHT){
            locatePOI();
            emptyGoals();
            computeNextSurveyAGoal(); // your map might have something new now.
        }

        if(!changedStateToTurnTowardsCoordinate(goals.peek())) {
            setTargetDistToGoal();
            st = STRAIGHT;
        }
    }

    prevSt = PATH_PLAN_SURVEY_A;
}

void Robot::pathPlanState() {
    haltNav();

    if(prevSt == STRAIGHT && isAtLastGoal()) {
        goals.pop();

        // if(housesVisited == 2) st = DONE;
        // else st = HOUSE;

        st = DONE;

        psoi.pop();
    } else {
        if(isAtGoal()) goals.pop();

        if(!changedStateToTurnTowardsCoordinate(goals.peek())) {
            setTargetDistToGoal();
            st = STRAIGHT;
        }
    }

    prevSt = PATH_PLAN;
}

void Robot::computeNextPOIGoal() {
    if(housesVisited == 2){ // go home
        goals = pathPlan(Coordinate{START_X, START_Y});
    }

    // find best adjacent tile to a house
    int bestScore = 20;

    Coordinate bestCoordinate;
    bestCoordinate.x = psoi.peek().x;
    bestCoordinate.y = psoi.peek().y;

    // Serial.println("----best coordinate before-----");
    // Serial.print(bestCoordinate.x);
    // Serial.print(",");
    // Serial.println(bestCoordinate.y);
    // Serial.println("---------");

    StackArray<Coordinate> n;
    neighbours(&n, bestCoordinate.x, bestCoordinate.y);

    // Serial.println("-----n count----");
    // Serial.println(n.count());
    // Serial.println("-----n count----");


    while(!n.isEmpty()){
        Coordinate nxt = n.pop();
        int score = heuristic(nxt, pos);

        if(score < bestScore) {
            bestCoordinate = nxt;
            bestScore = score;
        }
    }


    Serial.println("----best coordinate after-----");
    Serial.print(bestCoordinate.x);
    Serial.print(",");
    Serial.println(bestCoordinate.y);
    Serial.println("---------");

    // Serial.println("yahhhh");
    goals = pathPlan(bestCoordinate);
}

void Robot::printGoals() {
    Serial.println("");
    Serial.println("----- goals -------");
    StackArray<Coordinate> aux;

    while(!goals.isEmpty()) {
        Coordinate g = goals.pop();
        aux.push(g);

        Serial.println("");
        Serial.print("[");
        Serial.print(g.x);
        Serial.print(",");
        Serial.print(g.y);
        Serial.print("]");
        Serial.println("");
    }

    while(!aux.isEmpty()) {
        Coordinate g = aux.pop();
        goals.push(g);
    }

    Serial.println("");
}

void Robot::houseState() {
    haltNav();

    if(prevSt == PATH_PLAN){
        housesVisited += 1;
    }

    if(!changedStateToTurnTowardsCoordinate(psoi.peek())) {
        // targetDistToGoal = gravity.reading - HOUSE_PROXIMITY <---- main
        targetDistToGoal = HOUSE_PROXIMITY;
        navGoForward();

        identifyHouse() == Colour::RED ? inidicateRedHouse() : indiciateYellowHouse();

        targetDistToGoal = HOUSE_PROXIMITY; // targetDistToGoal = gravity.reading - HOUSE_PROXIMITY <---- main
        navGoReverse();

        computeNextPOIGoal();
        st = PATH_PLAN;
    }

    prevSt = HOUSE;
}

void Robot::straightState() {
    navGoForward();

    st = prevSt;
    prevSt = STRAIGHT;
    updateCurrentPosition();
}

void Robot::turnLeftState() {
    navTurnLeft();

    if(dir == North) {
        dir = West;
    } else {
        dir = static_cast<Direction>(static_cast<int>(dir) - 90);
    }


    st = prevSt;
    prevSt = TURN_LEFT;
}

void Robot::turnRightState() {
    navTurnRight();

    if(dir == West) {
        dir = North;
    } else {
        dir = static_cast<Direction>(static_cast<int>(dir) + 90);
    }

    st = prevSt;
    prevSt = TURN_RIGHT;
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

bool Robot::changedStateToTurnTowardsCoordinate(Coordinate c) {
    Coordinate nextGoal{c};
    bool turned = false;
    Direction ori = dir;

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
    int tilesRight;
    int tilesLeft;

    if(findObjectLeft(&tilesLeft)) {
        int poiX = pos.x - tilesLeft;
        int poiY = pos.y;

        Coordinate c{poiX, poiY};
        grid[poiY][poiX] = MISSION;
        psoi.push(c);

        // if(flameLeft.isFlameInSight()) {
        //     navTurnLeft();
        //     putOutFire();
        //     navTurnRight();

        //     // The poi was a candle.
        //     grid[poiY][poiX] = CANDLE;
        //     psoi.pop();
        // }
    }

    if(findObjectRight(&tilesRight)) {
        int poiX = pos.x + tilesRight;
        int poiY = pos.y;

        Coordinate c{poiX, poiY};
        grid[poiY][poiX] = MISSION;
        psoi.push(c);

        // if(flameRight.isFlameInSight()) {
        //     navTurnRight();
        //     putOutFire();
        //     navTurnLeft();

        //     // The poi was a candle.
        //     grid[poiY][poiX] = CANDLE;
        //     psoi.pop();
        // }
    }
}

bool Robot::findObjectRight(int *tiles) {
    *tiles = 5;
    return true;
}

bool Robot::findObjectLeft(int *tiles) {
    *tiles = 5;
    return true;
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
   return Colour::RED;
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

int Robot::heuristic(Coordinate a, Coordinate b) {
    return abs(a.x - b.x) + abs(b.y - b.y);
}

void Robot::neighbours(StackArray<Coordinate> *n, int currX, int currY) {
    if(currY > 0) { // above
        if(!(grid[currY-1][currX]) == WATER)
            n->push(Coordinate{currX, currY-1});
    }
    if(currY < MAP_HEIGHT-1) { // below
        if(!(grid[currY+1][currX]) == WATER)
            n->push(Coordinate{currX, currY+1});
    }
    if(currX > 0) { // left
        if(!(grid[currY][currX-1]) == WATER)
            n->push(Coordinate{currX-1, currY});
    }
    if(currX < MAP_WIDTH-1) { // right
        if(!(grid[currY][currX+1]) == WATER)
            n->push(Coordinate{currX+1, currY});
    }
}

StackArray<Coordinate> Robot::pathPlan(Coordinate e) {
    Serial.println("start");
    Serial.println(pos.x);
    Serial.println(pos.y);


    Serial.println("end");
    Serial.println(e.x);
    Serial.println(e.y);
    Serial.println("");

    Node costMap[MAP_WIDTH][MAP_HEIGHT];

    QueueArray<Coordinate> frontier;
    costMap[pos.y][pos.x].seen = true;
    costMap[pos.y][pos.x].cost = 0;
    costMap[pos.y][pos.x].parent = Coordinate{pos.x, pos.y};

    frontier.push(pos);

    while(!frontier.isEmpty()) {
        Coordinate current = frontier.pop();

        int currX = current.x;
        int currY = current.y;

        if(currX == e.x && currY == e.y) {
            break;
        }

        StackArray<Coordinate> n;
        neighbours(&n, currX, currY);

        // iterate over each neighbour
        while(!n.isEmpty()) {
            Coordinate nxt = n.pop();

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
        o = dir;
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

void Robot::navGoReverse() {
    return;
}

void Robot::navGoForward() {
    return;
}

void Robot::navTurnLeft() {
    return;
}

void Robot::navTurnRight() {
    return;
}

void Robot::emptyGoals() {
    while(!goals.isEmpty()) {
        goals.pop();
    }
}
