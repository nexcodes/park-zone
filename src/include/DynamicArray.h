#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>

template <typename T>
class DynamicArray {
private:
    T* data;
    int capacity;
    int size;
    
    void resize();
    
public:
    DynamicArray(int initialCapacity = 10);
    ~DynamicArray();
    
    void add(const T& item);
    T& get(int index);
    const T& get(int index) const;
    int getSize() const;
    void remove(int index);
    bool isEmpty() const;
    void clear();
    
    // Operator overloading for convenience
    T& operator[](int index);
    const T& operator[](int index) const;
};

// Template implementation in header (required for templates)
template <typename T>
DynamicArray<T>::DynamicArray(int initialCapacity) {
    capacity = initialCapacity;
    size = 0;
    data = new T[capacity];
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

template <typename T>
void DynamicArray<T>::resize() {
    capacity *= 2;
    T* newData = new T[capacity];
    for (int i = 0; i < size; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

template <typename T>
void DynamicArray<T>::add(const T& item) {
    if (size >= capacity) {
        resize();
    }
    data[size++] = item;
}

template <typename T>
T& DynamicArray<T>::get(int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}

template <typename T>
const T& DynamicArray<T>::get(int index) const {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}

template <typename T>
int DynamicArray<T>::getSize() const {
    return size;
}

template <typename T>
void DynamicArray<T>::remove(int index) {
    if (index < 0 || index >= size) {
        return;
    }
    for (int i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }
    size--;
}

template <typename T>
bool DynamicArray<T>::isEmpty() const {
    return size == 0;
}

template <typename T>
void DynamicArray<T>::clear() {
    size = 0;
}

template <typename T>
T& DynamicArray<T>::operator[](int index) {
    return get(index);
}

template <typename T>
const T& DynamicArray<T>::operator[](int index) const {
    return get(index);
}

#endif // DYNAMIC_ARRAY_H
