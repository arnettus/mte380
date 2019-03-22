#include <Navigator.h>

Navigator nav;

void setup() {
    nav.begin();

    delay(3000);
/*
    nav.goForward(30);
    nav.goForward(30);
    nav.goForward(30);
    delay(1000);
    nav.turnLeft();
    delay(1000);
    nav.goForward(30);
    delay(1000);
    nav.turnLeft();
    delay(1000);
    nav.goForward(30);
    nav.goForward(30);
    nav.goForward(30);
    delay(1000);
    nav.turnLeft();
    delay(1000);
    nav.goForward(30);
    delay(1000);
    nav.turnLeft();
    delay(1000);
*/

    nav.goForward(30);
    delay(1000);
    nav.turnLeft();
    delay(1000);
    nav.goForward(30);
    delay(1000);
    nav.turnRight();
    delay(1000);
    nav.goForward(60);
    delay(1000);
    nav.turnRight();
    delay(1000);
    nav.goForward(60);
    delay(1000);
    nav.turnRight();
    delay(1000);
    nav.goForward(30);
    delay(1000);
}

void loop() {

}
