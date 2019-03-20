#include <Motors.h>

// Results => 220 all the way round

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

Motors m;
Direction dir = Q;
char key;
Speed s(220, 220, 220, 220);

void setup() {
    Serial.begin(9600);
    s.print();
}

void loop() {
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
                if (dir != A) {
                    m.Halt();
                    Serial.println("Turning left");                    
                    delay(1000);
                    m.TurnLeft(s.fl, s.rl, s.fr, s.rr);
                    dir = A;
                }
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
                    m.TurnRight(s.fl, s.rl, s.fr, s.rr);
                    dir = D;
                }
                break;
            case 'q':
                if (dir != Q) {
                    Serial.println("Stopping");
                    m.Halt();
                    dir = Q;
                }
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
