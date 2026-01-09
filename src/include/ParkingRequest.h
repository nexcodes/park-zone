#ifndef PARKING_REQUEST_H
#define PARKING_REQUEST_H

#include "Enums.h"
#include "StateValidator.h"

class ParkingRequest {
private:
    int requestId;
    char* vehicleId;
    int requestedZoneId;
    int allocatedSlotId;
    RequestState currentState;
    long long requestTime;
    long long allocationTime;
    long long occupiedTime;
    long long releaseTime;
    bool crossZoneAllocation;
    
public:
    // Constructors & Destructor
    ParkingRequest(int id, const char* vehicle, int zone);
    ~ParkingRequest();
    
    // Getters
    int getRequestId() const;
    const char* getVehicleId() const;
    int getRequestedZoneId() const;
    int getAllocatedSlotId() const;
    RequestState getState() const;
    long long getRequestTime() const;
    long long getAllocationTime() const;
    long long getOccupiedTime() const;
    long long getReleaseTime() const;
    bool isCrossZone() const;
    
    // State Management
    bool setState(RequestState newState);
    bool allocateSlot(int slotId, bool isCrossZone);
    bool markOccupied();
    bool markReleased();
    bool cancel();
    
    // Analytics
    long long getParkingDuration() const;
    
    // Display
    void displayRequestInfo() const;
    const char* getStateString() const;
};

#endif // PARKING_REQUEST_H
