#ifndef ALLOCATION_ENGINE_H
#define ALLOCATION_ENGINE_H

#include "ParkingRequest.h"
#include "ParkingSlot.h"

// Forward declarations
class ParkingSystem;
class RollbackManager;

class AllocationEngine {
private:
    ParkingSystem* parkingSystem;
    RollbackManager* rollbackManager;
    float crossZonePenalty;
    
public:
    // Constructor
    AllocationEngine(ParkingSystem* system, RollbackManager* rbManager);
    
    // Allocation Methods
    bool allocateSlot(ParkingRequest* request);
    bool cancelAllocation(ParkingRequest* request);
    
    // Strategy
    ParkingSlot* findSlotInZone(int zoneId);
    ParkingSlot* findSlotInAdjacentZones(int requestedZoneId);
    
    // Configuration
    void setCrossZonePenalty(float penalty);
    float getCrossZonePenalty() const;
};

#endif // ALLOCATION_ENGINE_H
