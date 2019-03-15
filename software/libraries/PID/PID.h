#ifndef PID_H
#define PID_H

#include <math.h>
#include <assert.h>

struct PIDSettings {
    float dt;
    float kp;
    float ki;
    float kd;
    float outputMin;
    float outputMax;
    float tolerance;
};

class PID {
private:
    float dt;   // ms
    float kp;
    float ki;
    float kd;

    float outputMax;
    float outputMin;

    float integratedError;
    float integratedErrorMax;
    float integratedErrorMin;
    float prevError;
    float prevMeasured;

    float setpoint;
    float tolerance;    // percentage of setpoint

public:
    PID(float _dt, float _kp, float _ki, float _kd, float _outputMin, float _outputMax)
        : dt(_dt / 1000)
        , kp(_kp)
        , ki(_ki)
        , kd(_kd)
        , outputMin(_outputMin)
        , outputMax(_outputMax)
        , integratedErrorMax(0)
        , integratedErrorMin(0)
        , tolerance(0.1)
    {
        assert(dt > 0 && kp >= 0 && ki >= 0 && kd >= 0);
        if (ki > 0) {
            integratedErrorMax = outputMax / ki;
            integratedErrorMin = outputMin / ki;
        }
    }

    PID(const PIDSettings s)
        : dt(s.dt / 1000)
        , kp(s.kp)
        , ki(s.ki)
        , kd(s.kd)
        , outputMin(s.outputMin)
        , outputMax(s.outputMax)
        , integratedErrorMax(0)
        , integratedErrorMin(0)
        , tolerance(s.tolerance)
    {
        assert(dt > 0 && kp >= 0 && ki >= 0 && kd >= 0);
        if (ki > 0) {
            integratedErrorMax = outputMax / ki;
            integratedErrorMin = outputMin / ki;
        }
    }

    void setSetpoint(float _setpoint, float _tolerance) {
        setpoint  = _setpoint;
        tolerance = _tolerance;
    }

    bool hasReachedSetpoint() {
        return abs((prevMeasured - setpoint) / setpoint) <= tolerance;
    }

    void clearPreviousResults() {
        integratedError = 0;
        prevError = 0;
        prevMeasured = 0;
    }

    float compute(float measured) {
        float err = setpoint - measured;
        float integ = err * dt;
        float deriv;

        if (prevError != 0)
            deriv = (err - prevError) / dt;

        if (abs(integratedError + integ) > integratedErrorMin && abs(integratedError + integ) < integratedErrorMax)
            integratedError += integ;

        float computation = (err * kp) + (integratedError * ki) + (deriv * kd);

        int sgn = computation < 0 ? -1 : 1; // preserve sign in case of saturation

        if (abs(computation) > outputMax)
            computation = outputMax;
        else if (abs(computation) < outputMin)
            computation = outputMin;

        prevError = err;
        prevMeasured = measured;

        return computation * sgn;
    }
};

#endif
