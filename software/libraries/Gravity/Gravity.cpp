#include "Gravity.h"

Gravity::Gravity(const GravityConfig& config): cfg_(config), distance_(config.capacity)
{
    gravity_.init();
    gravity_.setTimeout(cfg_.timeout);
    gravity_.setAddress(cfg_.address);
    delay(500);

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

Terrain::TerrainType Gravity::GetTerrainType() {
    uint16_t distance = GetDistance();
    // int distance = gravity_.readRangeSingleMillimeters();
    if (distance >= cfg_.pitLow && distance <= cfg_.pitHigh) {
        return Terrain::PIT;
    } else if (distance >= cfg_.flatLow && distance <= cfg_.flatHigh) {
        return Terrain::FLAT;
    } else if (distance >= cfg_.sandLow && distance <= cfg_.sandHigh) {
        return Terrain::SAND;
    } else if (distance >= cfg_.gravelLow && distance <= cfg_.gravelHigh) {
        return Terrain::GRAV;
    }
    return Terrain::UNKN;
}

void Gravity::Reset() {
    distance_.reset();
}

GravitySensors::GravitySensors()
{}

GravitySensors::~GravitySensors() {
    for (int i = 0; i < 4; i++) {
        delete sensors_[i];
    }
}

void GravitySensors::Init()
{
    //edit these config values
    configs_[0] = {.capture = GravityConfig::NONE,
                   .range = GravityConfig::STANDARD,
                   .address = 0x31,
                   .reset_pin = -1,
                   .timeout = 500,
                   .capacity = 10,
                   .sandLow = 216,
                   .sandHigh = 236,
                   .pitLow = 279,
                   .pitHigh = 500,
                   .flatLow = 180,
                   .flatHigh = 189,
                   .gravelLow = 203,
                   .gravelHigh = 211};
    //edit these config values
    configs_[1] = {.capture = GravityConfig::NONE,
                   .range = GravityConfig::STANDARD,
                   .address = 0x32,
                   .reset_pin = 4,
                   .timeout = 500,
                   .capacity = 10,
                   .sandLow = 273,
                   .sandHigh = 292,
                   .pitLow = 349,
                   .pitHigh = 500,
                   .flatLow = 220,
                   .flatHigh = 235,
                   .gravelLow = 238,
                   .gravelHigh = 252};
    //edit these config values
    configs_[2] = {.capture = GravityConfig::NONE,
                   .range = GravityConfig::STANDARD,
                   .address = 0x33,
                   .reset_pin = 5,
                   .timeout = 500,
                   .capacity = 10,
                   .sandLow = 213,
                   .sandHigh = 220,
                   .pitLow = 270,
                   .pitHigh = 500,
                   .flatLow = 183,
                   .flatHigh = 189,
                   .gravelLow = 198,
                   .gravelHigh = 218};
    //edit these config values
    configs_[3] = {.capture = GravityConfig::NONE,
                   .range = GravityConfig::STANDARD,
                   .address = 0x34,
                   .reset_pin = 6,
                   .timeout = 500,
                   .capacity = 10,
                   .sandLow = 0,
                   .sandHigh = 0,
                   .pitLow = 0,
                   .pitHigh = 0,
                   .flatLow = 0,
                   .flatHigh = 0,
                   .gravelLow = 0,
                   .gravelHigh = 0};

    //didn't add a delay after these, it should work stillaz
    pinMode(configs_[1].reset_pin, OUTPUT);
    pinMode(configs_[2].reset_pin, OUTPUT);
    pinMode(configs_[3].reset_pin, OUTPUT);

    digitalWrite(configs_[1].reset_pin, LOW);
    digitalWrite(configs_[2].reset_pin, LOW);
    digitalWrite(configs_[3].reset_pin, LOW);

    sensors_[0] = new Gravity(configs_[0]);

    digitalWrite(configs_[1].reset_pin, HIGH);
    // delay(50);

    sensors_[1] = new Gravity(configs_[1]);

    digitalWrite(configs_[2].reset_pin, HIGH);
    // delay(50);

    sensors_[2] = new Gravity(configs_[2]);

    digitalWrite(configs_[3].reset_pin, HIGH);
    // delay(50);

    sensors_[3] = new Gravity(configs_[3]);
}

uint16_t GravitySensors::GetDistance(GravityDirection direction) {
    return sensors_[direction]->GetDistance();
}

Terrain::TerrainType GravitySensors::GetTerrainType(GravityDirection direction) {
    return sensors_[direction]->GetTerrainType();
}

void GravitySensors::Reset() {
    for (int i = 0; i < 4; i++) {
        sensors_[i]->Reset();
    }
}

void GravitySensors::PrintRaw()
{
    values_[0] = sensors_[0]->GetDistance();
    values_[1] = sensors_[1]->GetDistance();
    values_[2] = sensors_[2]->GetDistance();
    values_[3] = sensors_[3]->GetDistance();

    snprintf(msg_, 50, "g0=%u, g1=%u, g2=%u, g3=%u", values_[0], values_[1], values_[2], values_[3]);

    Serial.println(msg_);
}

void GravitySensors::PrintTile() {
    types_[0] = sensors_[0]->GetTerrainType();
    types_[1] = sensors_[1]->GetTerrainType();
    types_[2] = sensors_[2]->GetTerrainType();
    types_[3] = sensors_[3]->GetTerrainType();
    String s0 = Terrain::GetTerrainString(types_[0]);
    String s1 = Terrain::GetTerrainString(types_[1]);
    String s2 = Terrain::GetTerrainString(types_[2]);
    String s3 = Terrain::GetTerrainString(types_[3]);
    String final = "f0=" + s0 + " l1=" + s1 + " r2=" + s2 + " s3=" + s3;
    Serial.println(final);
}
