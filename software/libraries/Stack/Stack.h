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

    bool isEmpty() {
        return size == 0;
    }

    bool isFull() {
        return size == capacity;
    }

    void push(T value) {
        if (isFull()) {
            throw "Cannot add values to full stack.";
        }

        values[size] = value;
        size += 1;
    }

    T pop() {
        if (isEmpty()) {
            throw "Cannot pop values from empty stack.";
        }

        T val = values[size-1];
        size -= 1;

        return val;
    }
};
#endif
