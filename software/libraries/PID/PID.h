#ifndef PID_H
#define PID_H

#include <math.h>
#include <assert.h>

struct PIDSettings {
    float kp;
    float ki;
    float kd;
    float outputMin;
    float outputMax;
    float tolerance;
};

class PID {
private:
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
    float tolerance; // absolute tolerance for setpoint
    float _setpointUpper;
    float _setpointLower;

public:
    PID(float _kp, float _ki, float _kd, float _outputMin, float _outputMax)
        : kp(_kp)
        , ki(_ki)
        , kd(_kd)
        , outputMin(_outputMin)
        , outputMax(_outputMax)
        , integratedErrorMax(0)
        , integratedErrorMin(0)
        , tolerance(0.1)
    {
        assert(kp >= 0 && ki >= 0 && kd >= 0);
        if (ki > 0) {
            integratedErrorMax = outputMax / ki;
            integratedErrorMin = outputMin / ki;
        }
    }

    PID(const PIDSettings s)
        : kp(s.kp)
        , ki(s.ki)
        , kd(s.kd)
        , outputMin(s.outputMin)
        , outputMax(s.outputMax)
        , integratedErrorMax(0)
        , integratedErrorMin(0)
        , tolerance(s.tolerance)
    {
        assert(kp >= 0 && ki >= 0 && kd >= 0);
        if (ki > 0) {
            integratedErrorMax = outputMax / ki;
            integratedErrorMin = outputMin / ki;
        }
    }

    void setSetpoint(float _setpoint, float _tolerance) {
        setpoint  = _setpoint;
        tolerance = _tolerance;

        _setpointUpper = setpoint + tolerance;
        _setpointLower = setpoint - tolerance;
    }

    bool hasReachedSetpoint() {
        return abs(prevMeasured - setpoint) <= tolerance;
    }

    bool isGreaterThanSetpoint() {
        return prevMeasured >= _setpointLower;
    }

    bool isLessThanSetpoint() {
        return prevMeasured <= _setpointUpper;
    }

    void clearPreviousResults() {
        integratedError = 0;
        prevError = 0;
        prevMeasured = 0;
    }

    float compute(float measured) {
        float err = setpoint - measured;
        float deriv = 0;

        integratedError += err;
        if (abs(integratedError) > integratedErrorMax) {
            integratedError = integratedError > 0 ? integratedErrorMax : -1 * integratedErrorMax;
        }

        if (prevError != 0) {
            deriv = err - prevError;
            if (integratedError < 0)
                deriv *= -1;
        }

        float computation = (err * kp) + (integratedError * ki) + (deriv * kd);

        int sgn = computation < 0 ? -1 : 1; // preserve sign in case of saturation

        if (abs(computation) > outputMax)
            computation = outputMax * sgn;
        else if (abs(computation) < outputMin)
            computation = outputMin * sgn;

        prevError = err;
        prevMeasured = measured;

        return computation;
    }
};

#endif
