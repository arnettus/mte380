#ifndef TFLIDAR_H
#define TFLIDAR_H

#include <stdio.h>
#include <RollingAverage.h>

enum TFStatus {
    TF_STATUS_OKAY,
    TF_STATUS_ERROR_NO_DATA,
    TF_STATUS_ERROR_NO_HEADER,
    TF_STATUS_ERROR_BAD_CHECKSUM,
};

// Uses Serial1 on the Mega
class TFLidar {
private:
    RollingAverage distance;
    RollingAverage strength;

public:
    volatile unsigned long timestamp;

    TFLidar(uint64_t capacity);

    void     begin();
    TFStatus measure();
    uint64_t getDistance() const;
    uint64_t getDistanceRaw() const;
    uint64_t getStrength() const;
};

#endif
