#include "include/AllocationEngine.h"
#include "include/ParkingSystem.h"
#include "include/RollbackManager.h"
#include <iostream>

AllocationEngine::AllocationEngine(ParkingSystem* system, RollbackManager* rbManager) 
    : parkingSystem(system), rollbackManager(rbManager), crossZonePenalty(1.5f) {}

bool AllocationEngine::allocateSlot(ParkingRequest* request) {
    if (request == nullptr || request->getState() != REQUESTED) {
        std::cout << "Invalid request or not in REQUESTED state" << std::endl;
        return false;
    }
    
    int requestedZoneId = request->getRequestedZoneId();
    
    // Step 1: Try same zone first
    ParkingSlot* slot = findSlotInZone(requestedZoneId);
    
    if (slot != nullptr) {
        // Same-zone allocation
        slot->allocate(request->getVehicleId());
        request->allocateSlot(slot->getSlotId(), false);
        
        // Log for rollback
        rollbackManager->logAllocation(slot->getSlotId(), 
                                       request->getVehicleId(), 
                                       REQUESTED);
        
        std::cout << "Allocated slot " << slot->getSlotId() 
                  << " in requested zone " << requestedZoneId << std::endl;
        return true;
    }
    
    // Step 2: Try adjacent zones
    slot = findSlotInAdjacentZones(requestedZoneId);
    
    if (slot != nullptr) {
        // Cross-zone allocation
        slot->allocate(request->getVehicleId());
        request->allocateSlot(slot->getSlotId(), true);
        
        // Log for rollback
        rollbackManager->logAllocation(slot->getSlotId(), 
                                       request->getVehicleId(), 
                                       REQUESTED);
        
        std::cout << "Allocated slot " << slot->getSlotId() 
                  << " in zone " << slot->getZoneId()
                  << " (cross-zone from " << requestedZoneId << ")"
                  << " with " << (crossZonePenalty * 100) << "% penalty" << std::endl;
        return true;
    }
    
    std::cout << "No available slots found in zone " << requestedZoneId 
              << " or adjacent zones" << std::endl;
    return false;
}

bool AllocationEngine::cancelAllocation(ParkingRequest* request) {
    if (request == nullptr) {
        return false;
    }
    
    RequestState state = request->getState();
    if (state != REQUESTED && state != ALLOCATED) {
        std::cout << "Cannot cancel request in state: " 
                  << request->getStateString() << std::endl;
        return false;
    }
    
    // If slot was allocated, free it
    if (state == ALLOCATED) {
        int slotId = request->getAllocatedSlotId();
        ParkingSlot* slot = parkingSystem->findSlotById(slotId);
        if (slot != nullptr) {
            slot->release();
        }
    }
    
    return request->cancel();
}

ParkingSlot* AllocationEngine::findSlotInZone(int zoneId) {
    Zone* zone = parkingSystem->getZoneById(zoneId);
    if (zone == nullptr) {
        return nullptr;
    }
    
    return zone->findAvailableSlot();
}

ParkingSlot* AllocationEngine::findSlotInAdjacentZones(int requestedZoneId) {
    Zone* requestedZone = parkingSystem->getZoneById(requestedZoneId);
    if (requestedZone == nullptr) {
        return nullptr;
    }
    
    // Try each adjacent zone
    for (int i = 0; i < requestedZone->getAdjacentZoneCount(); i++) {
        int adjacentZoneId = requestedZone->getAdjacentZone(i);
        ParkingSlot* slot = findSlotInZone(adjacentZoneId);
        if (slot != nullptr) {
            return slot;
        }
    }
    
    return nullptr;
}

void AllocationEngine::setCrossZonePenalty(float penalty) {
    crossZonePenalty = penalty;
}

float AllocationEngine::getCrossZonePenalty() const {
    return crossZonePenalty;
}
