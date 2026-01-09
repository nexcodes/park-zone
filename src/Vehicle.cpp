#include "include/Vehicle.h"
#include <iostream>
#include <cstring>

Vehicle::Vehicle(const char* id, int preferredZone) 
    : preferredZoneId(preferredZone), ownerName(nullptr) {
    int len = strlen(id);
    vehicleId = new char[len + 1];
    strcpy(vehicleId, id);
}

Vehicle::Vehicle(const char* id, int preferredZone, const char* owner) 
    : preferredZoneId(preferredZone) {
    int len = strlen(id);
    vehicleId = new char[len + 1];
    strcpy(vehicleId, id);
    
    if (owner != nullptr) {
        len = strlen(owner);
        ownerName = new char[len + 1];
        strcpy(ownerName, owner);
    } else {
        ownerName = nullptr;
    }
}

Vehicle::~Vehicle() {
    delete[] vehicleId;
    if (ownerName != nullptr) {
        delete[] ownerName;
    }
}

const char* Vehicle::getVehicleId() const {
    return vehicleId;
}

int Vehicle::getPreferredZoneId() const {
    return preferredZoneId;
}

const char* Vehicle::getOwnerName() const {
    return ownerName;
}

void Vehicle::setPreferredZone(int zoneId) {
    preferredZoneId = zoneId;
}

void Vehicle::displayVehicleInfo() const {
    std::cout << "Vehicle ID: " << vehicleId 
              << ", Preferred Zone: " << preferredZoneId;
    if (ownerName != nullptr) {
        std::cout << ", Owner: " << ownerName;
    }
    std::cout << std::endl;
}
