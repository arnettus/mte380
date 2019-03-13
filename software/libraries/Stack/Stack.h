#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <string.h>

template <typename T>
class Stack {
private:
    T *values;
    int capacity;
    int size;

  public:
    Stack(int _capacity) : capacity(_capacity), size(0) {
        values = new T[_capacity];
    }

    ~Stack() {
        delete [] values;
    }

    int get_size() {
        return size;
    }

    void push(T value) {
        if size == capacity {
            // error handle
        }

        values[size] = value;
        size += 1;
    }

    T pop() {
        if size == 0 {
            // error handle
        }

        val = values[size-1];
        size -= 1;

        return val;
    }
};
#endif
