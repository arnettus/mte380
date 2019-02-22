#ifndef TFLIDAR_H
#define TFLIDAR_H

#include <stdio.h>
#include <SoftwareSerial.h>
#include <RollingAverage.h>

enum TFStatus {
    TF_STATUS_OKAY,
    TF_STATUS_ERROR_NO_DATA,
    TF_STATUS_ERROR_NO_HEADER,
    TF_STATUS_ERROR_BAD_CHECKSUM,
};

class TFLidar {
private:
    SoftwareSerial serial;
    RollingAverage distance;
    RollingAverage strength;

public:
    TFLidar(uint8_t rx, uint8_t tx, uint64_t capacity);

    void     begin();
    TFStatus measure();
    uint64_t getDistance();
    uint64_t getStrength();
};

#endif
