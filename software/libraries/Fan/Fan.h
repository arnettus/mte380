#ifndef FAN_H
#define FAN_H

#include <Arduino.h>
#include <Servo.h> 

class Fan {
public:
    enum FanMode {
        OFF = 0,
        ARM = 1000,
        LOW_SPEED = 1100,
        MED_SPEED = 1200,
        HIGH_SPEED = 1500
    };
    Fan(int pin = 10, int min = 1000, int max = 2000);
    void Setup(int delay_ms = 500);
    bool TurnOn(FanMode fan_mode = LOW_SPEED);
    bool TurnOff();
    bool Shutdown();
private:
    Servo esc_;
    int pin_, min_, max_;
    FanMode fan_mode_;
    bool is_armed_;
    bool Set(FanMode fan_mode);
};

#endif // FAN_H