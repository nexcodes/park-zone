#ifndef ZONE_H
#define ZONE_H

#include "ParkingArea.h"
#include "DynamicArray.h"

class Zone {
private:
    int zoneId;
    char* zoneName;
    DynamicArray<ParkingArea*> areas;
    DynamicArray<int> adjacentZoneIds;
    
public:
    // Constructors & Destructor
    Zone(int id, const char* name);
    ~Zone();
    
    // Getters
    int getZoneId() const;
    const char* getZoneName() const;
    int getAreaCount() const;
    int getTotalSlots() const;
    int getOccupiedSlots() const;
    float getUtilization() const;
    
    // Area Management
    void addParkingArea(ParkingArea* area);
    ParkingArea* getAreaById(int areaId);
    ParkingArea* getAreaByIndex(int index);
    
    // Adjacency Management
    void addAdjacentZone(int zoneId);
    bool isAdjacentTo(int zoneId) const;
    int getAdjacentZoneCount() const;
    int getAdjacentZone(int index) const;
    
    // Slot Allocation
    ParkingSlot* findAvailableSlot();
    
    // Display
    void displayZoneInfo() const;
};

#endif // ZONE_H
