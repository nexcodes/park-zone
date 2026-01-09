#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <cstring>

// Rollback operation structure
struct RollbackOperation {
    int slotId;
    bool previousAvailability;
    int previousRequestState;  // Store as int to avoid circular dependency
    char* vehicleId;
    long long timestamp;
    
    RollbackOperation() : slotId(-1), previousAvailability(true), 
                         previousRequestState(0), vehicleId(nullptr), timestamp(0) {}
    
    ~RollbackOperation() {
        if (vehicleId != nullptr) {
            delete[] vehicleId;
        }
    }
    
    // Copy constructor
    RollbackOperation(const RollbackOperation& other) {
        slotId = other.slotId;
        previousAvailability = other.previousAvailability;
        previousRequestState = other.previousRequestState;
        timestamp = other.timestamp;
        
        if (other.vehicleId != nullptr) {
            int len = strlen(other.vehicleId);
            vehicleId = new char[len + 1];
            strcpy(vehicleId, other.vehicleId);
        } else {
            vehicleId = nullptr;
        }
    }
    
    // Assignment operator
    RollbackOperation& operator=(const RollbackOperation& other) {
        if (this != &other) {
            // Clean up existing vehicleId
            if (vehicleId != nullptr) {
                delete[] vehicleId;
            }
            
            slotId = other.slotId;
            previousAvailability = other.previousAvailability;
            previousRequestState = other.previousRequestState;
            timestamp = other.timestamp;
            
            if (other.vehicleId != nullptr) {
                int len = strlen(other.vehicleId);
                vehicleId = new char[len + 1];
                strcpy(vehicleId, other.vehicleId);
            } else {
                vehicleId = nullptr;
            }
        }
        return *this;
    }
};

class Stack {
private:
    RollbackOperation* data;
    int capacity;
    int top;
    
    void resize();
    
public:
    Stack(int initialCapacity = 100);
    ~Stack();
    
    void push(const RollbackOperation& operation);
    RollbackOperation pop();
    RollbackOperation peek() const;
    bool isEmpty() const;
    bool isFull() const;
    int getSize() const;
};

#endif // STACK_H
