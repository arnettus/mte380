#ifndef TFLIDAR_H
#define TFLIDAR_H

#include <stdio.h>
#include <Arduino.h>
#include <RollingAverage.h>

enum TFStatus {
    TF_STATUS_OKAY,
    TF_STATUS_ERROR_NO_DATA,
    TF_STATUS_ERROR_NO_HEADER,
    TF_STATUS_ERROR_BAD_CHECKSUM,
    TF_STATUS_NOT_CONNECTED,
};

class TFLidar {
private:
    HardwareSerial *serial;
    RollingAverage<uint16_t> distance;
    RollingAverage<uint16_t> strength;
    unsigned long timestamp;

public:
    TFLidar(int capacity);

    void     begin(HardwareSerial *s);
    TFStatus measure();
    uint16_t getDistance() const;
    uint16_t getDistanceRaw() const;
    uint16_t getStrength() const;
    unsigned long getTimestamp();
    void printLatest(HardwareSerial *s, bool useRawValues) const;
};

#endif
