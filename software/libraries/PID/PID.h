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
    bool  useKpOnMeasure;
};

class PID {
private:
    float kp;
    float ki;
    float kd;

    float outputMin;
    float outputMax;

    float integratedError;
    float integratedErrorMin;
    float integratedErrorMax;

    float prevMeasured;

    bool  useKpOnMeasure;
    float offsetFromFirstMeasurement;

    float setpoint;
    float tolerance;
    float _setpointUpper;
    float _setpointLower;

public:
    PID(float _kp, float _ki, float _kd, float _outputMin, float _outputMax)
        : kp(_kp)
        , ki(_ki)
        , kd(_kd)
        , outputMin(_outputMin)
        , outputMax(_outputMax)
        , integratedError(0)
        , tolerance(0)
        , useKpOnMeasure(false)
        , offsetFromFirstMeasurement(0)
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
        , integratedError(0)
        , tolerance(s.tolerance)
        , useKpOnMeasure(s.useKpOnMeasure)
        , offsetFromFirstMeasurement(0)
    {
        assert(kp >= 0 && ki >= 0 && kd >= 0);
        if (ki > 0) {
            integratedErrorMax = outputMax / ki;
            integratedErrorMin = outputMin / ki;
        }
    }

    void begin(float _setpoint, float firstMeasurement) {
        setpoint = _setpoint;

        _setpointUpper = setpoint + tolerance;
        _setpointLower = setpoint - tolerance;

        prevMeasured = firstMeasurement;

        integratedError = 0;
        offsetFromFirstMeasurement = 0;
    }

    float compute(float measured) {
        float err = setpoint - measured;
        float proportionalTerm = err;
        
        integratedError += err;
        if (abs(integratedError) > integratedErrorMax)
            integratedError = integratedError > 0 ? integratedErrorMax : -1 * integratedErrorMax;
        else if (abs(integratedError) < integratedErrorMin)
            integratedError = integratedError > 0 ? integratedErrorMin : -1 * integratedErrorMin;

        if (useKpOnMeasure) {
            offsetFromFirstMeasurement += measured - prevMeasured;
            proportionalTerm = -1 * offsetFromFirstMeasurement;
        }

        float computation = (kp * proportionalTerm) + (ki * integratedError) - (kd * (measured - prevMeasured));
        int sgn = computation < 0 ? -1 : 1;

        if (abs(computation) > outputMax)
            computation = outputMax * sgn;
        else if (abs(computation) < outputMin)
            computation = outputMin * sgn;

        prevMeasured = measured;

        return computation;
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
};

#endif
