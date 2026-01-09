#include "include/ParkingSlot.h"
#include <iostream>
#include <cstring>

ParkingSlot::ParkingSlot(int id, int area, int zone) 
    : slotId(id), areaId(area), zoneId(zone), status(SLOT_AVAILABLE), 
      allocatedVehicleId(nullptr), allocationTime(0) {}

ParkingSlot::~ParkingSlot() {
    if (allocatedVehicleId != nullptr) {
        delete[] allocatedVehicleId;
    }
}

int ParkingSlot::getSlotId() const {
    return slotId;
}

int ParkingSlot::getAreaId() const {
    return areaId;
}

int ParkingSlot::getZoneId() const {
    return zoneId;
}

SlotStatus ParkingSlot::getStatus() const {
    return status;
}

const char* ParkingSlot::getAllocatedVehicleId() const {
    return allocatedVehicleId;
}

bool ParkingSlot::isAvailable() const {
    return status == SLOT_AVAILABLE;
}

long long ParkingSlot::getAllocationTime() const {
    return allocationTime;
}

bool ParkingSlot::allocate(const char* vehicleId) {
    if (!isAvailable()) {
        return false;
    }
    
    // Copy vehicle ID
    if (vehicleId != nullptr) {
        int len = strlen(vehicleId);
        allocatedVehicleId = new char[len + 1];
        strcpy(allocatedVehicleId, vehicleId);
    }
    
    status = SLOT_OCCUPIED;
    allocationTime = time(nullptr);
    return true;
}

bool ParkingSlot::release() {
    if (status != SLOT_OCCUPIED) {
        return false;
    }
    
    if (allocatedVehicleId != nullptr) {
        delete[] allocatedVehicleId;
        allocatedVehicleId = nullptr;
    }
    
    status = SLOT_AVAILABLE;
    allocationTime = 0;
    return true;
}

bool ParkingSlot::setStatus(SlotStatus newStatus) {
    status = newStatus;
    return true;
}

void ParkingSlot::displayInfo() const {
    std::cout << "Slot ID: " << slotId 
              << ", Zone: " << zoneId 
              << ", Area: " << areaId 
              << ", Status: " << (isAvailable() ? "AVAILABLE" : "OCCUPIED");
    
    if (allocatedVehicleId != nullptr) {
        std::cout << ", Vehicle: " << allocatedVehicleId;
    }
    std::cout << std::endl;
}
