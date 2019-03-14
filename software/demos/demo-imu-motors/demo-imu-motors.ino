#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Motors.h>

// LEFT TURN: front left: 190, rear left: 195, front right: 190, rear right: 195
// LEFT TURN: front left: 180, rear left: 185, front right: 180, rear right: 185 w/ bag of 3 rocks
// RIGHT TURN: front left: 185, rear left: 180, front right: 185, rear right: 180 w/ bag of 3 rocks
// RIGHT TURN: front left: 170, rear left: 167, front right: 170, rear right: 167 w/ bag of 3 rocks
// RIGHT TURN: 170, 167, 170, 167

enum Direction {
    W,
    A,
    S,
    D,
    Q,
};

struct Speed {
    char msg[70];

    int fl;
    int rl;
    int fr;
    int rr;

    Speed(int _fl, int _rl, int _fr, int _rr) : fl(_fl), rl(_rl), fr(_fr), rr(_rr) {}
    void print() {
        snprintf(msg, 70, "front left: %d, rear left: %d, front right: %d, rear right: %d", fl, rl, fr, rr);
        Serial.println(msg);
    }
};

const Speed RIGHT_TURN_PARAMS(170, 167, 170, 167);
const Speed LEFT_TURN_PARAMS(167, 170, 167, 170);

Adafruit_BNO055 bno = Adafruit_BNO055(55);
Motors m;
Direction dir = Q;
char key;
Speed s(200, 200, 200, 200);

void setup(void)
{
  Serial.begin(9600);
  if (!bno.begin())
  {
    Serial.print("No BNO055 detected");
    while (1);
  }

  char msg[64];
  uint8_t sys, gyro, accel, mag;
  bno.getCalibration(&sys, &gyro, &accel, &mag);
  while (sys != 3 || gyro != 3 || mag != 3 || accel != 3) {
    delay(2000);
    bno.getCalibration(&sys, &gyro, &accel, &mag);
    snprintf(msg, 64, "sys: %u, gyro: %u, accel: %u, mag: %u", sys, gyro, accel, mag);
    Serial.println(msg);
  }

  s.print();

  delay(1000);
}

void loop(void)
{
    sensors_event_t ori;
    bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);

    if (Serial.available() > 0) {
        key = Serial.read();

        switch (key) {
            case 'w':
                if (dir != W) {
                    m.Halt();
                    Serial.println("Going forward");
                    delay(1000);
                    m.DriveFwd();
                    dir = W;
                }
                break;
            case 'a':
                m.Halt();
                Serial.println("Turning left");                    
                delay(1000);
                turnLeft(80);
                dir = Q;
                break;
            case 's':
                if (dir != S) {
                    m.Halt();
                    Serial.println("Going reverse");
                    delay(1000);
                    m.DriveRev();
                    dir = S;
                }
                break;
            case 'd':
                if (dir != D) {
                    m.Halt();
                    Serial.println("Turning right");
                    delay(1000);
                    turnRight(76);
                    dir = Q;
                }
                break;
            case 'q':
                if (dir != Q) {
                    Serial.println("Stopping");
                    m.Halt();
                    dir = Q;
                }
                break;
            case 'n':
                s = LEFT_TURN_PARAMS;
                s.print();
                break;
            case 'm':
                s = RIGHT_TURN_PARAMS;
                s.print();
                break;
            case 'y':
                s.fl++;
                s.print();
                break;
            case 'u':
                s.fr++;
                s.print();
                break;
            case 'h':
                s.rl++;
                s.print();
                break;
            case 'j':
                s.rr++;
                s.print();
                break;
            case 'i':
                s.fl--;
                s.print();
                break;
            case 'o':
                s.fr--;
                s.print();
                break;
            case 'k':
                s.rl--;
                s.print();
                break;
            case 'l':
                s.rr--;
                s.print();
                break;
            default:
                break;
        }
    }
}

void turnLeft(float deg) {
    sensors_event_t ori;
    bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);
    float start = ori.orientation.x;

    m.TurnLeft(s.fl, s.rl, s.fr, s.rr);

    //Serial.println(start);
    //Serial.println(deg);

    if (start >= deg) {
        while (true) {
            //delay(10);
            bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);
            //Serial.println(ori.orientation.x);
            if (start - ori.orientation.x >= deg)
                break;
        }
    } else {
        float target = deg - start - 10;
        while (true) {
            //delay(10);
            bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);
            //Serial.println(ori.orientation.x);
            if ((ori.orientation.x > (start + 5)) && (360 - ori.orientation.x > target))
                break;
        }
    }

    //Serial.println("DONE");

    m.Halt();
}

void turnRight(float deg) {
    sensors_event_t ori;
    bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);
    float start = ori.orientation.x;

    m.TurnRight(s.fl, s.rl, s.fr, s.rr);
    //Serial.println(start);
    //Serial.println(deg);

    if (360 - start >= deg) {
        while (true) {
            //delay(10);
            bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);
            //Serial.println(ori.orientation.x);
            if (ori.orientation.x - start >= deg)
                break;
        }
    } else {
        float target = deg - (360 - start);
        while (true) {
            //delay(10);
            bno.getEvent(&ori, Adafruit_BNO055::VECTOR_EULER);
            //Serial.println(ori.orientation.x);
            if ((ori.orientation.x < deg) && (ori.orientation.x > target))
                break;;
        }
    }

    //Serial.println("DONE");

    m.Halt();
}
