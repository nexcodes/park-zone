#ifndef PARKING_AREA_H
#define PARKING_AREA_H

#include "ParkingSlot.h"

class ParkingArea {
private:
    int areaId;
    int zoneId;
    char* areaName;
    ParkingSlot** slots;
    int totalSlots;
    int occupiedSlots;
    
public:
    // Constructors & Destructor
    ParkingArea(int id, int zone, const char* name, int slotCount);
    ~ParkingArea();
    
    // Getters
    int getAreaId() const;
    int getZoneId() const;
    const char* getAreaName() const;
    int getTotalSlots() const;
    int getOccupiedSlots() const;
    int getAvailableSlots() const;
    float getUtilization() const;
    
    // Slot Management
    ParkingSlot* findFirstAvailableSlot();
    ParkingSlot* getSlotById(int slotId);
    bool allocateSlot(int slotId, const char* vehicleId);
    bool releaseSlot(int slotId);
    
    // Statistics
    void updateOccupancyCount();
    void displayAreaInfo() const;
};

#endif // PARKING_AREA_H
