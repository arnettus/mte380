/*
 * Reference 1: http://wiki.seeedstudio.com/Grove-TF_Mini_LiDAR/#resources
 * Reference 2: https://github.com/TFmini/TFmini-Arduino/blob/master/TFmini_Arduino_SoftwareSerial/TFmini_Arduino_SoftwareSerial.ino
 */

#include "TFLidar.h"

const long TF_BAUDRATE       = 115200;
const int TF_HEADER         = 0x59;
const int TF_PACKET_LENGTH  = 9;

TFLidar::TFLidar(int capacity) : serial(NULL), distance(capacity), strength(capacity) {};

void TFLidar::begin(HardwareSerial *s) {
    serial = s;
    serial->begin(TF_BAUDRATE);
}

TFStatus TFLidar::measure() {
    if (serial == NULL)
        return TF_STATUS_NOT_CONNECTED;

    if (!serial->available())
        return TF_STATUS_ERROR_NO_DATA;

    unsigned long ts = micros();
    int tries = 0;

    while (tries < TF_PACKET_LENGTH) {
        if (serial->read() == TF_HEADER) {
            int checksum = 0;
            int data[9] = {0};

            data[0] = TF_HEADER;
            data[1] = serial->read();

            if (data[1] != TF_HEADER)
                return TF_STATUS_ERROR_NO_HEADER;

            for (int i = 2; i < 9; i++)
                data[i] = serial->read();

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

uint16_t TFLidar::getDistance() const {
    return distance.read();
}

uint16_t TFLidar::getStrength() const {
    return strength.read();
}

unsigned long TFLidar::getTimestamp() {
    return timestamp;
}

void TFLidar::printLatest(HardwareSerial *s) const {
    if (s == NULL)
        return;

    s->print("Distance: ");
    s->print(distance.read());
	s->print(", ");

    s->print("Strength: ");
    s->println(strength.read());
}
