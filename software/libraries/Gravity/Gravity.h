#ifndef GRAVITY_H
#define GRAVITY_H

#include <Arduino.h>
#include "VL53L0X.h"
#include "RollingAverage.h"
#include "Terrain.h"

struct GravityConfig {
    enum Capture {
        HIGH_SPEED,
        HIGH_ACCURACY,
        NONE
    };
    enum Range {
        STANDARD,
        LONG_RANGE
    };

    Capture capture;
    Range range;
    int address, reset_pin, timeout, capacity, min_range, max_range;
};

class Gravity {
public:
    enum Status {
        IN_RANGE,
        TOO_CLOSE,
        TOO_FAR
    };
    Gravity();
    Gravity(const GravityConfig& config);
    void Init();
    uint16_t GetDistance() const;
    Status GetAndCheckDistance(uint16_t& distance);
    void Reset();
private:
    VL53L0X gravity_;
    RollingAverage<uint16_t> distance_;
    GravityConfig cfg_;
};

#endif // GRAVITY_H
