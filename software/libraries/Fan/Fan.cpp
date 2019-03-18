#include "Fan.h"

Fan::Fan(int pin, int min, int max)
    : pin_(pin)
    , min_(min)
    , max_(max)
    , fan_mode_(Fan::FanMode::OFF)
    , is_armed_(false) {
}

void Fan::Setup(int delay_ms) {
    esc_.attach(pin_, min_, max_);
    fan_mode_ = Fan::FanMode::ARM;
    esc_.writeMicroseconds(fan_mode_);
    delay(delay_ms);
}

bool Fan::Set(FanMode mode) {
    if (fan_mode_ == Fan::FanMode::OFF) {
        return false;
    }

    fan_mode_ = mode;
    esc_.writeMicroseconds(mode);
    return true;
}

bool Fan::TurnOn(FanMode mode) {
    return Set(mode);
}

bool Fan::TurnOff() {
    return Set(Fan::FanMode::ARM);
}

bool Fan::Shutdown() {
    return Set(Fan::FanMode::OFF);
}