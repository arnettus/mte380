#ifndef IR_H
#define IR_H

#include <Arduino.h>
#include <RollingAverage.h>

struct IRConfig {
    bool is_left;
    int pin, capacity, min_range, max_range;
};

class IR {
public:
    enum FlameStatus {
        NO_FLAME,
        FLAME_DETECTED,
    };
    IR(const IRConfig& config);
    bool IsLeft();
    int ReadValue();
    FlameStatus ReadFlame();
private:
    RollingAverage<int> values_;
    IRConfig cfg_;
};

class Firewall {
public:
    enum FlameLocation {
        NOT_FOUND,
        LEFT_FLAME,
        RIGHT_FLAME
    };
    Firewall();
    Firewall(IRConfig configs[], int num);
    ~Firewall();
    int ReadValue(int index);
    IR::FlameStatus ReadFlame(int index);
    FlameLocation GetFlameLocation();
private:
    int num_;
    IR* sensors_[4];
    IRConfig default_cfgs_[4];
};

#endif // IR_H
