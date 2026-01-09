#include "include/ParkingArea.h"
#include <iostream>
#include <cstring>

ParkingArea::ParkingArea(int id, int zone, const char* name, int slotCount) 
    : areaId(id), zoneId(zone), totalSlots(slotCount), occupiedSlots(0) {
    
    // Copy area name
    int len = strlen(name);
    areaName = new char[len + 1];
    strcpy(areaName, name);
    
    // Create slots array
    slots = new ParkingSlot*[totalSlots];
    for (int i = 0; i < totalSlots; i++) {
        int slotId = areaId * 1000 + i;  // Unique slot ID
        slots[i] = new ParkingSlot(slotId, areaId, zoneId);
    }
}

ParkingArea::~ParkingArea() {
    delete[] areaName;
    for (int i = 0; i < totalSlots; i++) {
        delete slots[i];
    }
    delete[] slots;
}

int ParkingArea::getAreaId() const {
    return areaId;
}

int ParkingArea::getZoneId() const {
    return zoneId;
}

const char* ParkingArea::getAreaName() const {
    return areaName;
}

int ParkingArea::getTotalSlots() const {
    return totalSlots;
}

int ParkingArea::getOccupiedSlots() const {
    return occupiedSlots;
}

int ParkingArea::getAvailableSlots() const {
    return totalSlots - occupiedSlots;
}

float ParkingArea::getUtilization() const {
    if (totalSlots == 0) return 0.0f;
    return (float)occupiedSlots / totalSlots;
}

ParkingSlot* ParkingArea::findFirstAvailableSlot() {
    for (int i = 0; i < totalSlots; i++) {
        if (slots[i]->isAvailable()) {
            return slots[i];
        }
    }
    return nullptr;
}

ParkingSlot* ParkingArea::getSlotById(int slotId) {
    for (int i = 0; i < totalSlots; i++) {
        if (slots[i]->getSlotId() == slotId) {
            return slots[i];
        }
    }
    return nullptr;
}

bool ParkingArea::allocateSlot(int slotId, const char* vehicleId) {
    ParkingSlot* slot = getSlotById(slotId);
    if (slot == nullptr) return false;
    
    if (slot->allocate(vehicleId)) {
        occupiedSlots++;
        return true;
    }
    return false;
}

bool ParkingArea::releaseSlot(int slotId) {
    ParkingSlot* slot = getSlotById(slotId);
    if (slot == nullptr) return false;
    
    if (slot->release()) {
        occupiedSlots--;
        return true;
    }
    return false;
}

void ParkingArea::updateOccupancyCount() {
    occupiedSlots = 0;
    for (int i = 0; i < totalSlots; i++) {
        if (!slots[i]->isAvailable()) {
            occupiedSlots++;
        }
    }
}

void ParkingArea::displayAreaInfo() const {
    std::cout << "Area: " << areaName 
              << " (ID: " << areaId << ")"
              << ", Zone: " << zoneId
              << ", Total Slots: " << totalSlots
              << ", Occupied: " << occupiedSlots
              << ", Available: " << getAvailableSlots()
              << ", Utilization: " << (getUtilization() * 100) << "%"
              << std::endl;
}
