#include "IR.h"

IR::IR(const IRConfig& config) : cfg_(config), values_(config.capacity) {}

bool IR::IsLeft() {
    return cfg_.is_left;
}

int IR::ReadValue() {
    values_.add(analogRead(cfg_.pin));
    return values_.read();
}

IR::FlameStatus IR::ReadFlame() {
    int value = ReadValue();
    if (value >= cfg_.min_range && value <= cfg_.max_range) {
        return FLAME_DETECTED;
    }
    return NO_FLAME;
}

Firewall::Firewall() : num_(4) {
    default_cfgs_[0] = {
        is_left: true,
        pin : A0,
        capacity : 10,
        min_range :0,
        max_range : 1000
    };

    default_cfgs_[1] = {
        .is_left = false,
        .pin = A1,
        .capacity = 10,
        .min_range = 0,
        .max_range = 1000
    };

    default_cfgs_[2] = {
        .is_left = true,
        .pin = A2,
        .capacity = 10,
        .min_range = 0,
        .max_range = 1000
    };

    default_cfgs_[3] = {
        .is_left = false,
        .pin = A3,
        .capacity = 10,
        .min_range = 0,
        .max_range = 1000
    };

    for (int i = 0; i < num_; i++) {
        sensors_[i] = new IR(default_cfgs_[i]);
    }
}

Firewall::Firewall(IRConfig configs[], int num) : num_(num) {
    for (int i = 0; i < num; i++) {
        sensors_[i] = new IR(configs[i]);
    }
}

Firewall::~Firewall() {
    for (int i = 0; i < num_; i++) {
        delete sensors_[i];
    }
}

int Firewall::ReadValue(int index) {
    return sensors_[index]->ReadValue();
}

IR::FlameStatus Firewall::ReadFlame(int index) {
    return sensors_[index]->ReadFlame();
}

Firewall::FlameLocation Firewall::GetFlameLocation() {
    // TODO(jskhu): Add a check for checking left or right
    if (sensors_[0]->ReadFlame() == IR::FlameStatus::FLAME_DETECTED
        && sensors_[2]->ReadFlame() == IR::FlameStatus::FLAME_DETECTED) {
            return LEFT_FLAME;
    } else if (sensors_[1]->ReadFlame() == IR::FlameStatus::FLAME_DETECTED
               && sensors_[3]->ReadFlame() == IR::FlameStatus::FLAME_DETECTED) {
            return RIGHT_FLAME;
    }
    return NOT_FOUND;
}
