/*
 * Reference 1: http://wiki.seeedstudio.com/Grove-TF_Mini_LiDAR/#resources
 * Reference 2: https://github.com/TFmini/TFmini-Arduino/blob/master/TFmini_Arduino_SoftwareSerial/TFmini_Arduino_SoftwareSerial.ino
 */

#include "TFLidar.h"
#include <Arduino.h>

const int TF_BAUDRATE       = 115200;
const int TF_HEADER         = 0x59;
const int TF_PACKET_LENGTH  = 9;

const uint64_t MIN_DISTANCE = 0;
const long MAX_DISTANCE = 500;

// Assumes the raw distance values ranges from 0 to 0xffff
inline uint64_t rawToDistance(uint64_t val) {
    return (val * (MAX_DISTANCE - MIN_DISTANCE) / 0xffff) + MIN_DISTANCE;
}

TFLidar::TFLidar(uint64_t capacity) : distance(capacity), strength(capacity) {};

void TFLidar::begin() {
    Serial1.begin(TF_BAUDRATE);
}

TFStatus TFLidar::measure() {
    if (!Serial1.available())
        return TF_STATUS_ERROR_NO_DATA;

    unsigned long ts = micros();
    int tries = 0;

    while (tries < TF_PACKET_LENGTH) {
        if (Serial1.read() == TF_HEADER) {
            int checksum = 0;
            int data[9] = {0};

            data[0] = TF_HEADER;
            data[1] = Serial1.read();

            if (data[1] != TF_HEADER)
                return TF_STATUS_ERROR_NO_HEADER;

            for (int i = 2; i < 9; i++)
                data[i] = Serial1.read();

            checksum = (data[0] + data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7]) & 0xff;
            if (data[8] != checksum)
                return TF_STATUS_ERROR_BAD_CHECKSUM;

            distance.add(data[2] + (data[3] << 8));
            strength.add(data[4] + (data[5] << 8));

            timestamp = ts;

            return TF_STATUS_OKAY;
        }
    }

    return TF_STATUS_ERROR_NO_HEADER;
}

uint64_t TFLidar::getDistance() const {
    return rawToDistance(distance.read());
}

uint64_t TFLidar::getDistanceRaw() const {
    return distance.read();
}

uint64_t TFLidar::getStrength() const {
    return strength.read();
}
