#include "include/Stack.h"
#include <stdexcept>

Stack::Stack(int initialCapacity) {
    capacity = initialCapacity;
    top = -1;
    data = new RollbackOperation[capacity];
}

Stack::~Stack() {
    delete[] data;
}

void Stack::resize() {
    capacity *= 2;
    RollbackOperation* newData = new RollbackOperation[capacity];
    for (int i = 0; i <= top; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

void Stack::push(const RollbackOperation& operation) {
    if (isFull()) {
        resize();
    }
    data[++top] = operation;
}

RollbackOperation Stack::pop() {
    if (isEmpty()) {
        throw std::underflow_error("Stack is empty");
    }
    return data[top--];
}

RollbackOperation Stack::peek() const {
    if (isEmpty()) {
        throw std::underflow_error("Stack is empty");
    }
    return data[top];
}

bool Stack::isEmpty() const {
    return top == -1;
}

bool Stack::isFull() const {
    return top == capacity - 1;
}

int Stack::getSize() const {
    return top + 1;
}
