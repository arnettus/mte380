#ifndef IR_H
#define IR_H

#include <Arduino.h>

class IR {
private:
    const int pin;

public:
    IR(int pin);

    int readDistance();
};

#endif
