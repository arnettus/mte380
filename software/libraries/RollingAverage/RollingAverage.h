#ifndef ROLLING_AVERAGE_H
#define ROLLING_AVERAGE_H

#include <stdio.h>
#include <stdint.h>

class RollingAverage {
private:
    uint64_t *values;
    uint64_t capacity;
    uint64_t currentSum;
    uint64_t currentNumberOfEntries;
    int currentIndex;

public:
    RollingAverage(uint64_t _capacity);
    ~RollingAverage();

    uint64_t read();
    void add(uint64_t value);
    void reset(uint64_t newCapacity); // pass in 0 to keep the current capacity
    void print();
};

#endif
