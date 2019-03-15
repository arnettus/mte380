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

        // Cheating.
        T read(int idx);

    public:
        Stack(int _capacity);
        ~Stack();

        int getSize();
        bool isEmpty();
        bool isFull();

        void push(T value);
        void empty();
        T pop();
        T peek();

        bool operator==(Stack<T> rhs);
};

template<typename T>
Stack<T>::Stack(int _capacity) : capacity(_capacity), size(0) {
    values = new T[_capacity];
}

template<typename T>
Stack<T>::~Stack() {
    delete [] values;
}

template<typename T>
int Stack<T>::getSize() {
    return size;
}

template<typename T>
bool Stack<T>::isEmpty() {
    return size == 0;
}

template<typename T>
bool Stack<T>::isFull() {
    return size == capacity;
}

template<typename T>
void Stack<T>::push(T value) {
    if (isFull()) {
        throw "Cannot add values to full stack.";
    }

    values[size] = value;
    size += 1;
}

template<typename T>
void Stack<T>::empty() {
    size = 0;
}

template<typename T>
T Stack<T>::pop() {
    if (isEmpty()) {
        throw "Cannot pop values from empty stack.";
    }

    T val = values[size-1];
    size -= 1;

    return val;
}

template<typename T>
T Stack<T>::peek() {
    if (isEmpty()) {
        throw "Cannot peek value from empty stack.";
    }

    T val = values[size-1];

    return val;
}

template<typename T>
bool Stack<T>::operator==(Stack<T> rhs) {
    if(rhs.getSize() != getSize()) {
        return false;
    }

    for(int i = 0; i <size; i++) {
        if(read(i) != rhs.read(i)){
            return false;
        }
    }

    return true;
}

#endif
