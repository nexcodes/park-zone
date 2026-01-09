#ifndef ROLLBACK_MANAGER_H
#define ROLLBACK_MANAGER_H

#include "Stack.h"

// Forward declarations
class ParkingSystem;
class ParkingSlot;
class ParkingRequest;

class RollbackManager {
private:
    Stack operationStack;
    ParkingSystem* system;
    
public:
    // Constructor
    RollbackManager(ParkingSystem* sys);
    ~RollbackManager();
    
    // Operation Logging
    void logAllocation(int slotId, const char* vehicleId, int prevState);
    
    // Rollback
    bool rollback(int k);
    bool canRollback() const;
    int getRollbackDepth() const;
    
    // Utility
    void clearHistory();
};

#endif // ROLLBACK_MANAGER_H
