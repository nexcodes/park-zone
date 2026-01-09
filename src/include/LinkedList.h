#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        
        Node(const T& value) : data(value), next(nullptr) {}
    };
    
    Node* head;
    Node* tail;
    int size;
    
public:
    LinkedList();
    ~LinkedList();
    
    void append(const T& item);
    void prepend(const T& item);
    T* find(bool (*predicate)(const T&));
    void traverse(void (*callback)(const T&));
    int getSize() const;
    bool isEmpty() const;
    void clear();
    
    // Iterator support
    class Iterator {
    private:
        Node* current;
    public:
        Iterator(Node* node) : current(node) {}
        T& operator*() { return current->data; }
        Iterator& operator++() { current = current->next; return *this; }
        bool operator!=(const Iterator& other) { return current != other.current; }
    };
    
    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
};

// Template implementation
template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

template <typename T>
void LinkedList<T>::append(const T& item) {
    Node* newNode = new Node(item);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

template <typename T>
void LinkedList<T>::prepend(const T& item) {
    Node* newNode = new Node(item);
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    size++;
}

template <typename T>
T* LinkedList<T>::find(bool (*predicate)(const T&)) {
    Node* current = head;
    while (current != nullptr) {
        if (predicate(current->data)) {
            return &(current->data);
        }
        current = current->next;
    }
    return nullptr;
}

template <typename T>
void LinkedList<T>::traverse(void (*callback)(const T&)) {
    Node* current = head;
    while (current != nullptr) {
        callback(current->data);
        current = current->next;
    }
}

template <typename T>
int LinkedList<T>::getSize() const {
    return size;
}

template <typename T>
bool LinkedList<T>::isEmpty() const {
    return size == 0;
}

template <typename T>
void LinkedList<T>::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    size = 0;
}

#endif // LINKED_LIST_H
