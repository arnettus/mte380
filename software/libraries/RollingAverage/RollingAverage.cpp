#include "RollingAverage.h"
#include <stdlib.h>
#include <string.h>

RollingAverage::RollingAverage(uint64_t _capacity) {
    capacity = _capacity;
    currentSum = 0;
    currentIndex = 0;
    currentNumberOfEntries = 0;

    values = (uint64_t *)malloc(sizeof(uint64_t) * _capacity);

    if (values == NULL)
        printf("RollingAverage:: malloc failed\n");

    memset(values, 0, sizeof(uint64_t) * _capacity);
}

RollingAverage::~RollingAverage() {
    free(values);
}

void RollingAverage::add(uint64_t value) {
    currentSum -= values[currentIndex];
    currentSum += value;
    values[currentIndex] = value;
    currentIndex = (currentIndex + 1) % capacity;

    if (currentNumberOfEntries < capacity)
        currentNumberOfEntries++;
}

uint64_t RollingAverage::read() {
    return currentNumberOfEntries ? (currentSum + (currentNumberOfEntries / 2)) / currentNumberOfEntries : 0;
}

void RollingAverage::reset(uint64_t newCapacity) {
    if (newCapacity) {
        free(values);
        values = (uint64_t *)malloc(sizeof(uint64_t) * newCapacity);

        if (values == NULL)
            printf("RollingAverage:: malloc failed\n");

        capacity = newCapacity;
    }

    memset(values, 0, sizeof(uint64_t) * capacity);
    currentSum = 0;
    currentIndex = 0;
    currentNumberOfEntries = 0;
}

void RollingAverage::print() {
    printf("Values: ");
    for (int i = 0; i < capacity - 1; i++)
        printf("%llu-", values[i]);
    printf("%llu\n", values[capacity - 1]);

    printf("Average: %llu [%llu/%llu]\n", read(), currentSum, currentNumberOfEntries);
}
