#include "include/RollbackManager.h"
#include "include/ParkingSystem.h"
#include <iostream>
#include <cstring>
#include <ctime>

RollbackManager::RollbackManager(ParkingSystem* sys) : system(sys) {}

RollbackManager::~RollbackManager() {}

void RollbackManager::logAllocation(int slotId, const char* vehicleId, int prevState) {
    RollbackOperation op;
    op.slotId = slotId;
    op.previousAvailability = true;  // Slot was available before allocation
    op.previousRequestState = prevState;
    op.timestamp = time(nullptr);
    
    if (vehicleId != nullptr) {
        int len = strlen(vehicleId);
        op.vehicleId = new char[len + 1];
        strcpy(op.vehicleId, vehicleId);
    }
    
    operationStack.push(op);
}

bool RollbackManager::rollback(int k) {
    if (k <= 0 || operationStack.isEmpty()) {
        return false;
    }
    
    int rolledBack = 0;
    for (int i = 0; i < k && !operationStack.isEmpty(); i++) {
        RollbackOperation op = operationStack.pop();
        
        // Find and restore slot
        ParkingSlot* slot = system->findSlotById(op.slotId);
        if (slot != nullptr) {
            slot->release();  // Free the slot
        }
        
        // Find and restore request
        if (op.vehicleId != nullptr) {
            ParkingRequest* request = system->findRequestByVehicle(op.vehicleId);
            if (request != nullptr) {
                // Restore previous state
                RequestState prevState = static_cast<RequestState>(op.previousRequestState);
                request->setState(prevState);
            }
        }
        
        rolledBack++;
    }
    
    std::cout << "Rolled back " << rolledBack << " operation(s)" << std::endl;
    return true;
}

bool RollbackManager::canRollback() const {
    return !operationStack.isEmpty();
}

int RollbackManager::getRollbackDepth() const {
    return operationStack.getSize();
}

void RollbackManager::clearHistory() {
    while (!operationStack.isEmpty()) {
        operationStack.pop();
    }
}
