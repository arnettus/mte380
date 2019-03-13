#ifndef ROLLING_AVERAGE_H
#define ROLLING_AVERAGE_H

#include <string.h>

#ifdef NON_ARDUINO
#include <iostream>
#endif

template <typename T>
class RollingAverage {
private:
    T *values;
    int capacity;
    int currentIndex;
    T currentSum;
    int currentNumberOfEntries;
    unsigned long timestamp;

public:
    RollingAverage(int _capacity) : capacity(_capacity), currentSum(0), currentIndex(0), currentNumberOfEntries(0) {
        values = new T[_capacity];
        memset(values, 0, sizeof(T) * _capacity);
    }

    ~RollingAverage() {
        delete [] values;
    }

    T read() {
        return currentNumberOfEntries ? (currentSum + (currentNumberOfEntries / 2)) / currentNumberOfEntries : 0;
    }

    void add(T value) {
        currentSum -= values[currentIndex];
        currentSum += value;
        values[currentIndex] = value;
        currentIndex = (currentIndex + 1) % capacity;

        if (currentNumberOfEntries < capacity)
            currentNumberOfEntries++;
    }

    void reset() {
        memset(values, 0, sizeof(T) * capacity);
        currentSum = 0;
        currentIndex = 0;
        currentNumberOfEntries = 0;
    }

    void reset(int newCapacity) {
        if (newCapacity) {
            delete [] values;
            values = new T[newCapacity];
            capacity = newCapacity;
        }

        memset(values, 0, sizeof(T) * capacity);
        currentSum = 0;
        currentIndex = 0;
        currentNumberOfEntries = 0;

    }

#ifdef NON_ARDUINO
    void print() {
        std::cout << "Values: ";
        for (int i = 0; i < capacity - 1; i++)
            std::cout << values[i] << ", ";
        std::cout << values[capacity - 1] << std::endl;

        std::cout << "Average: " << read() << " [" << currentSum << "/" << currentNumberOfEntries << "]"<< std::endl;
    }
#endif

};
#endif
