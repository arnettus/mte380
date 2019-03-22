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
    st = PATH_PLAN_SURVEY_A;
    prevSt = STRAIGHT;
    pos = Coordinate{START_X, START_Y};

    housesVisited = 0;
    foodFound = false;

    dir = North;

    Coordinate f1{1, 0};
    Coordinate f2{4, 0};
    Coordinate f3{2, 4};

    foodpsoi.push(f3);
    foodpsoi.push(f1);
    foodpsoi.push(f2);

    while(st != DONE) {
        printState();
        printGoals();

        if(st == PATH_PLAN_SURVEY_A) {
            pathPlanSurveyAStateSpecial();
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

        //if(foodFound) st = DONE;
        delay(1000);
        //st = DONE;
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
    Serial.println(prevSt);
    Serial.print(" dir: ");
    Serial.println(dir);
    Serial.println("----- END STATE ---- ");
}

void Robot::initializeLidar() {}

void Robot::initializeFireFighter() {}

void Robot::shutDownFireFighter() {}

void Robot::initializeColour() {}

void Robot::initializeNavigator() {}

void Robot::haltNav() {
    return;
}

void Robot::pathPlanSurveyAStateSpecial() {
    while(!isOnRow(1)) {
        haltNav();
        if(!isOnRow(5)) locatePOI();
        navGoForward(); // go 30;

        pos.y = pos.y - 1;
    }

    locatePOI();

    prevSt = PATH_PLAN_SURVEY_A;
    st = PATH_PLAN;

    computeNextPOIGoal();
}

void Robot::computeNextPOIGoal() {
    if(!foodFound) {
        emptyGoals();

        Coordinate food{foodpsoi.peek().x, foodpsoi.peek().y};

        pathPlan(&goals, food);
    } else if(!(housesVisited == 2)) {
        emptyGoals();

        int bestScore = 20;

        Coordinate bestCoordinate;
        bestCoordinate.x = psoi.peek().x;
        bestCoordinate.y = psoi.peek().y;

        StackArray<Coordinate> n;
        neighbours(&n, bestCoordinate.x, bestCoordinate.y);

        while(!n.isEmpty()){
            Coordinate nxt = n.pop();
            int score = heuristic(nxt, pos);

            if(score < bestScore) {
                bestCoordinate = nxt;
                bestScore = score;
            }
        }

        pathPlan(&goals, bestCoordinate);
    } else {
        emptyGoals();
        pathPlan(&goals, Coordinate{START_X, START_Y});
    }
}

void Robot::pathPlanState() {
    haltNav();

    if(prevSt == STRAIGHT && isAtLastGoal()) {
        Coordinate g = goals.pop();
        if(g.x == START_X && g.y == START_Y && !foodFound && housesVisited == 2) {
            st = DONE;
            prevSt = PATH_PLAN;

            return;
        }

        if(!foodFound) {
            foodpsoi.pop();

            detectFood(); // check magnotometer reading

            computeNextPOIGoal(); // go to house or food again
        } else if(housesVisited == 2) { // go home
            emptyGoals();
            goals.push(Coordinate{pos.x, pos.y});
        } else { // else you're at a house
            housesVisited += 1;

            Coordinate h{psoi.peek().x, psoi.peek().y}; // house of concern
            psoi.pop();

            housePsoi.push(h);

            st = HOUSE;
        }
    } else {
        if(isAtGoal()) {
            goals.pop();
        }

        bool turnaz = changedStateToTurnTowardsCoordinate(goals.peek());

        if(!turnaz) {
            setTargetDistToGoal();
            st = STRAIGHT;
        }
    }

    prevSt = PATH_PLAN;
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
    Serial.println("------- end goals ------");
}

void Robot::detectFood() {
    if(pos.x == 2 && pos.y == 4) {
        Serial.print("FOUND FOOOOOOD");
        foodFound = true;
    }
}

void Robot::houseState() {
    haltNav();

    if(!changedStateToTurnTowardsCoordinate(housePsoi.peek())) {
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
    return goals.peek().x == pos.x && goals.peek().y == pos.y;
}

bool Robot::isAtLastGoal() {
    return isAtGoal() && goals.count() == 1;
}

bool Robot::isOnRow(int y) {
    return pos.y == y;
}

bool Robot::changedStateToTurnTowardsCoordinate(Coordinate c) {
    bool turned = false;
    Direction ori = dir;

    if(c.x > pos.x) {
        if(ori != East) {
            if(ori == North) st = TURN_RIGHT;
            else st = TURN_LEFT;

            turned = true;
        }
    } else if(c.x < pos.x) {
        if(ori != West) {
            if(ori == South) st = TURN_RIGHT;
            else st = TURN_LEFT;

            turned = true;
        }
    } else if(c.y > pos.y) {
        if(ori != South) {
            if(ori == West) st = TURN_LEFT;
            else st = TURN_RIGHT;

            turned = true;
        }
    } else if(c.y < pos.y) {
        if(ori != North) {
            if(ori == East) st = TURN_LEFT;
            else st = TURN_RIGHT;

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
    // when the row is 3
    if(pos.y == 3) {
        grid[3][4] = MISSION;
        psoi.push(Coordinate{4,3});
    }

    // add to psoi
    // when the row is 2
    if(pos.y == 2) {
        grid[1][2] = MISSION;
        psoi.push(Coordinate{2,1});
    }

    // add to psoi

    // int tilesRight;
    // int tilesLeft;

    // if(findObjectLeft(&tilesLeft)) {
    //     int poiX = pos.x - tilesLeft;
    //     int poiY = pos.y;

    //     Coordinate c{poiX, poiY};
    //     grid[poiY][poiX] = MISSION;
    //     psoi.push(c);

    //     // if(flameLeft.isFlameInSight()) {
    //     //     navTurnLeft();
    //     //     putOutFire();
    //     //     navTurnRight();

    //     //     // The poi was a candle.
    //     //     grid[poiY][poiX] = CANDLE;
    //     //     psoi.pop();
    //     // }
    // }

    // if(findObjectRight(&tilesRight)) {
    //     int poiX = pos.x + tilesRight;
    //     int poiY = pos.y;

    //     Coordinate c{poiX, poiY};
    //     grid[poiY][poiX] = MISSION;
    //     psoi.push(c);

    //     // if(flameRight.isFlameInSight()) {
    //     //     navTurnRight();
    //     //     putOutFire();
    //     //     navTurnLeft();

    //     //     // The poi was a candle.
    //     //     grid[poiY][poiX] = CANDLE;
    //     //     psoi.pop();
    //     // }
    // }
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
    goals.push(Coordinate{pos.x, pos.y-1});
}

int Robot::heuristic(Coordinate a, Coordinate b) {
    return abs(a.x - b.x) + abs(b.y - b.y);
}

void Robot::neighbours(StackArray<Coordinate> *n, int currX, int currY) {
    if(currY > 0) { // above
        if((grid[currY-1][currX]) == FLAT)
            n->push(Coordinate{currX, currY-1});
    }
    if(currY < MAP_HEIGHT-1) { // below
        if((grid[currY+1][currX]) == FLAT)
            n->push(Coordinate{currX, currY+1});
    }
    if(currX > 0) { // left
        if((grid[currY][currX-1]) == FLAT)
            n->push(Coordinate{currX-1, currY});
    }
    if(currX < MAP_WIDTH-1) { // right
        if((grid[currY][currX+1]) == FLAT)
            n->push(Coordinate{currX+1, currY});
    }
}

void Robot::pathPlan(StackArray<Coordinate> *path, Coordinate e) {
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

        // Coordinate p = costMap[focus.y][focus.x].parent;

        // Direction nextDir = dirFromParent(p, focus);

        // if(nextDir != focusDir) path.push(focus);

        // focus = p;
        // focusDir = nextDir;

    while(!(focus.x == pos.x && focus.y == pos.y)){
        Coordinate p = costMap[focus.y][focus.x].parent;
        Direction nextDir = dirFromParent(p, focus);

        if(nextDir != focusDir) path->push(focus);
        focus.x = p.x;
        focus.y = p.y;
        focusDir = nextDir;
    }
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

void Robot::setTargetDistToGoal() {
    return;
}

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
