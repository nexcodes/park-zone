#include "include/Zone.h"
#include <iostream>
#include <cstring>

Zone::Zone(int id, const char* name) : zoneId(id) {
    int len = strlen(name);
    zoneName = new char[len + 1];
    strcpy(zoneName, name);
}

Zone::~Zone() {
    delete[] zoneName;
    // Delete all parking areas
    for (int i = 0; i < areas.getSize(); i++) {
        delete areas[i];
    }
}

int Zone::getZoneId() const {
    return zoneId;
}

const char* Zone::getZoneName() const {
    return zoneName;
}

int Zone::getAreaCount() const {
    return areas.getSize();
}

int Zone::getTotalSlots() const {
    int total = 0;
    for (int i = 0; i < areas.getSize(); i++) {
        total += areas[i]->getTotalSlots();
    }
    return total;
}

int Zone::getOccupiedSlots() const {
    int occupied = 0;
    for (int i = 0; i < areas.getSize(); i++) {
        occupied += areas[i]->getOccupiedSlots();
    }
    return occupied;
}

float Zone::getUtilization() const {
    int total = getTotalSlots();
    if (total == 0) return 0.0f;
    return (float)getOccupiedSlots() / total;
}

void Zone::addParkingArea(ParkingArea* area) {
    if (area != nullptr) {
        areas.add(area);
    }
}

ParkingArea* Zone::getAreaById(int areaId) {
    for (int i = 0; i < areas.getSize(); i++) {
        if (areas[i]->getAreaId() == areaId) {
            return areas[i];
        }
    }
    return nullptr;
}

ParkingArea* Zone::getAreaByIndex(int index) {
    if (index >= 0 && index < areas.getSize()) {
        return areas[index];
    }
    return nullptr;
}

void Zone::addAdjacentZone(int zoneId) {
    // Check if already exists
    for (int i = 0; i < adjacentZoneIds.getSize(); i++) {
        if (adjacentZoneIds[i] == zoneId) {
            return;
        }
    }
    adjacentZoneIds.add(zoneId);
}

bool Zone::isAdjacentTo(int zoneId) const {
    for (int i = 0; i < adjacentZoneIds.getSize(); i++) {
        if (adjacentZoneIds[i] == zoneId) {
            return true;
        }
    }
    return false;
}

int Zone::getAdjacentZoneCount() const {
    return adjacentZoneIds.getSize();
}

int Zone::getAdjacentZone(int index) const {
    if (index >= 0 && index < adjacentZoneIds.getSize()) {
        return adjacentZoneIds[index];
    }
    return -1;
}

ParkingSlot* Zone::findAvailableSlot() {
    for (int i = 0; i < areas.getSize(); i++) {
        ParkingSlot* slot = areas[i]->findFirstAvailableSlot();
        if (slot != nullptr) {
            return slot;
        }
    }
    return nullptr;
}

void Zone::displayZoneInfo() const {
    std::cout << "\n=== Zone: " << zoneName << " (ID: " << zoneId << ") ===" << std::endl;
    std::cout << "Total Slots: " << getTotalSlots() << std::endl;
    std::cout << "Occupied: " << getOccupiedSlots() << std::endl;
    std::cout << "Available: " << (getTotalSlots() - getOccupiedSlots()) << std::endl;
    std::cout << "Utilization: " << (getUtilization() * 100) << "%" << std::endl;
    
    std::cout << "\nParking Areas:" << std::endl;
    for (int i = 0; i < areas.getSize(); i++) {
        std::cout << "  ";
        areas[i]->displayAreaInfo();
    }
    
    if (adjacentZoneIds.getSize() > 0) {
        std::cout << "\nAdjacent Zones: ";
        for (int i = 0; i < adjacentZoneIds.getSize(); i++) {
            std::cout << adjacentZoneIds[i];
            if (i < adjacentZoneIds.getSize() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
}
