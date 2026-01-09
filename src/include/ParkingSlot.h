#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

#include "Enums.h"
#include <ctime>

class ParkingSlot {
private:
    int slotId;
    int areaId;
    int zoneId;
    SlotStatus status;
    char* allocatedVehicleId;
    long long allocationTime;
    
public:
    // Constructors & Destructor
    ParkingSlot(int id, int area, int zone);
    ~ParkingSlot();
    
    // Getters
    int getSlotId() const;
    int getAreaId() const;
    int getZoneId() const;
    SlotStatus getStatus() const;
    const char* getAllocatedVehicleId() const;
    bool isAvailable() const;
    long long getAllocationTime() const;
    
    // State Management
    bool allocate(const char* vehicleId);
    bool release();
    bool setStatus(SlotStatus newStatus);
    
    // Display
    void displayInfo() const;
};

#endif // PARKING_SLOT_H
