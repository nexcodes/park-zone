#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>

template <typename T>
class Queue {
private:
    T* data;
    int capacity;
    int front;
    int rear;
    int size;
    
    void resize();
    
public:
    Queue(int initialCapacity = 50);
    ~Queue();
    
    void enqueue(const T& item);
    T dequeue();
    T peek() const;
    bool isEmpty() const;
    bool isFull() const;
    int getSize() const;
};

// Template implementation
template <typename T>
Queue<T>::Queue(int initialCapacity) {
    capacity = initialCapacity;
    front = 0;
    rear = -1;
    size = 0;
    data = new T[capacity];
}

template <typename T>
Queue<T>::~Queue() {
    delete[] data;
}

template <typename T>
void Queue<T>::resize() {
    int newCapacity = capacity * 2;
    T* newData = new T[newCapacity];
    
    for (int i = 0; i < size; i++) {
        newData[i] = data[(front + i) % capacity];
    }
    
    delete[] data;
    data = newData;
    front = 0;
    rear = size - 1;
    capacity = newCapacity;
}

template <typename T>
void Queue<T>::enqueue(const T& item) {
    if (isFull()) {
        resize();
    }
    rear = (rear + 1) % capacity;
    data[rear] = item;
    size++;
}

template <typename T>
T Queue<T>::dequeue() {
    if (isEmpty()) {
        throw std::underflow_error("Queue is empty");
    }
    T item = data[front];
    front = (front + 1) % capacity;
    size--;
    return item;
}

template <typename T>
T Queue<T>::peek() const {
    if (isEmpty()) {
        throw std::underflow_error("Queue is empty");
    }
    return data[front];
}

template <typename T>
bool Queue<T>::isEmpty() const {
    return size == 0;
}

template <typename T>
bool Queue<T>::isFull() const {
    return size == capacity;
}

template <typename T>
int Queue<T>::getSize() const {
    return size;
}

#endif // QUEUE_H
