#include "Gravity.h"

GravityConfig default_config = {.capture = GravityConfig::NONE,
                                .range = GravityConfig::STANDARD,
                                .address = 0x29,
                                .reset_pin = 40,
                                .timeout = 500,
                                .capacity = 10,
                                .min_range = 60,
                                .max_range = 400
};

Gravity::Gravity(): cfg_(default_config), distance_(default_config.capacity) {}

Gravity::Gravity(const GravityConfig& config): cfg_(config), distance_(config.capacity) {}

void Gravity::Init() {
    gravity_.init();
    gravity_.setTimeout(cfg_.timeout);
    gravity_.setAddress(cfg_.address);

    switch(cfg_.range) {
        case GravityConfig::LONG_RANGE:
            gravity_.setSignalRateLimit(0.1);
            gravity_.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
            gravity_.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
            break;
        default:
            break;
    }

    switch(cfg_.capture) {
        case GravityConfig::HIGH_SPEED:
            gravity_.setMeasurementTimingBudget(20000);
            break;
        case GravityConfig::HIGH_ACCURACY:
            // gravity_.setMeasurementTimingBudget(200000);
            break;
    }
}

uint16_t Gravity::GetDistance() const {
    distance_.add(gravity_.readRangeSingleMillimeters());
    return distance_.read();
}

Gravity::Status Gravity::GetAndCheckDistance(uint16_t& distance) {
    distance = GetDistance();
    if (distance < cfg_.min_range) {
        return TOO_CLOSE;
    } else if (distance > cfg_.max_range) {
        return TOO_FAR;
    }
    return IN_RANGE;
}

void Gravity::Reset() {
    distance_.reset();
}
