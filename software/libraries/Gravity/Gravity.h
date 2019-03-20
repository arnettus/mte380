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
    int address, reset_pin, timeout, capacity;
    uint16_t sandLow, sandHigh, pitLow, pitHigh, flatLow, flatHigh;
    uint16_t gravelLow, gravelHigh;
    //add detection thresholds, these will prolly be constant 
};

class Gravity {
public:
    Gravity(const GravityConfig& config);
    uint16_t GetDistance() const;
    Terrain::TerrainType GetTerrainType();
    void Reset();
private:
    VL53L0X gravity_;
    RollingAverage<uint16_t> distance_;
    GravityConfig cfg_;
};

class GravitySensors {
public:
    GravitySensors();
    ~GravitySensors();
    enum GravityDirection {
        FRONT = 0,
        LEFT = 1,
        RIGHT = 2,
        VERTICAL = 3
    };
    void Init();
    uint16_t GetDistance(GravityDirection direction);
    Terrain::TerrainType GetTerrainType(GravityDirection direction);
    void Reset();
    void PrintRaw();
    void PrintTile();
private:
    GravityConfig configs_ [4];
    Gravity* sensors_ [4];
    int values_ [4];
    Terrain::TerrainType types_[4];
    char msg_ [50];
};

#endif // GRAVITY_H
