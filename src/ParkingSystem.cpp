#include "include/ParkingSystem.h"
#include <iostream>
#include <cstring>

ParkingSystem::ParkingSystem() : nextRequestId(1) {
    rollbackManager = new RollbackManager(this);
    allocationEngine = new AllocationEngine(this, rollbackManager);
}

ParkingSystem::~ParkingSystem() {
    // Delete zones
    for (int i = 0; i < zones.getSize(); i++) {
        delete zones[i];
    }
    
    // Delete requests
    for (int i = 0; i < requests.getSize(); i++) {
        delete requests[i];
    }
    
    delete allocationEngine;
    delete rollbackManager;
}

void ParkingSystem::addZone(Zone* zone) {
    if (zone != nullptr) {
        zones.add(zone);
    }
}

Zone* ParkingSystem::getZoneById(int zoneId) {
    for (int i = 0; i < zones.getSize(); i++) {
        if (zones[i]->getZoneId() == zoneId) {
            return zones[i];
        }
    }
    return nullptr;
}

int ParkingSystem::getZoneCount() const {
    return zones.getSize();
}

int ParkingSystem::createRequest(const char* vehicleId, int zoneId) {
    ParkingRequest* request = new ParkingRequest(nextRequestId, vehicleId, zoneId);
    requests.add(request);
    
    std::cout << "Created request ID: " << nextRequestId 
              << " for vehicle " << vehicleId 
              << " in zone " << zoneId << std::endl;
    
    return nextRequestId++;
}

bool ParkingSystem::processRequest(int requestId) {
    ParkingRequest* request = getRequestById(requestId);
    if (request == nullptr) {
        std::cout << "Request " << requestId << " not found" << std::endl;
        return false;
    }
    
    return allocationEngine->allocateSlot(request);
}

bool ParkingSystem::cancelRequest(int requestId) {
    ParkingRequest* request = getRequestById(requestId);
    if (request == nullptr) {
        return false;
    }
    
    bool success = allocationEngine->cancelAllocation(request);
    
    if (success) {
        // Log cancelled trip
        TripHistory trip;
        trip.requestId = requestId;
        int len = strlen(request->getVehicleId());
        trip.vehicleId = new char[len + 1];
        strcpy(trip.vehicleId, request->getVehicleId());
        trip.zoneId = request->getRequestedZoneId();
        trip.startTime = request->getRequestTime();
        trip.endTime = time(nullptr);
        trip.completed = false;
        tripHistory.append(trip);
    }
    
    return success;
}

ParkingRequest* ParkingSystem::getRequestById(int requestId) {
    for (int i = 0; i < requests.getSize(); i++) {
        if (requests[i]->getRequestId() == requestId) {
            return requests[i];
        }
    }
    return nullptr;
}

bool ParkingSystem::markVehicleArrived(int requestId) {
    ParkingRequest* request = getRequestById(requestId);
    if (request == nullptr) {
        return false;
    }
    
    if (request->getState() != ALLOCATED) {
        std::cout << "Request must be in ALLOCATED state" << std::endl;
        return false;
    }
    
    return request->markOccupied();
}

bool ParkingSystem::markVehicleExited(int requestId) {
    ParkingRequest* request = getRequestById(requestId);
    if (request == nullptr) {
        return false;
    }
    
    if (request->getState() != OCCUPIED) {
        std::cout << "Request must be in OCCUPIED state" << std::endl;
        return false;
    }
    
    // Release the slot
    int slotId = request->getAllocatedSlotId();
    ParkingSlot* slot = findSlotById(slotId);
    if (slot != nullptr) {
        slot->release();
    }
    
    // Mark request as released
    bool success = request->markReleased();
    
    if (success) {
        // Log completed trip
        TripHistory trip;
        trip.requestId = requestId;
        int len = strlen(request->getVehicleId());
        trip.vehicleId = new char[len + 1];
        strcpy(trip.vehicleId, request->getVehicleId());
        trip.zoneId = request->getRequestedZoneId();
        trip.startTime = request->getOccupiedTime();
        trip.endTime = request->getReleaseTime();
        trip.completed = true;
        tripHistory.append(trip);
    }
    
    return success;
}

bool ParkingSystem::rollbackOperations(int k) {
    return rollbackManager->rollback(k);
}

ParkingSlot* ParkingSystem::findSlotById(int slotId) {
    for (int i = 0; i < zones.getSize(); i++) {
        for (int j = 0; j < zones[i]->getAreaCount(); j++) {
            ParkingArea* area = zones[i]->getAreaByIndex(j);
            if (area != nullptr) {
                ParkingSlot* slot = area->getSlotById(slotId);
                if (slot != nullptr) {
                    return slot;
                }
            }
        }
    }
    return nullptr;
}

ParkingRequest* ParkingSystem::findRequestByVehicle(const char* vehicleId) {
    for (int i = 0; i < requests.getSize(); i++) {
        if (strcmp(requests[i]->getVehicleId(), vehicleId) == 0) {
            return requests[i];
        }
    }
    return nullptr;
}

float ParkingSystem::getAverageParkingDuration() {
    long long totalDuration = 0;
    int count = 0;
    
    for (auto it = tripHistory.begin(); it != tripHistory.end(); ++it) {
        if ((*it).completed) {
            totalDuration += ((*it).endTime - (*it).startTime);
            count++;
        }
    }
    
    if (count == 0) return 0.0f;
    return (float)totalDuration / count;
}

float ParkingSystem::getZoneUtilization(int zoneId) {
    Zone* zone = getZoneById(zoneId);
    if (zone == nullptr) return 0.0f;
    return zone->getUtilization();
}

float ParkingSystem::getSystemUtilization() {
    int totalSlots = 0;
    int occupiedSlots = 0;
    
    for (int i = 0; i < zones.getSize(); i++) {
        totalSlots += zones[i]->getTotalSlots();
        occupiedSlots += zones[i]->getOccupiedSlots();
    }
    
    if (totalSlots == 0) return 0.0f;
    return (float)occupiedSlots / totalSlots;
}

int ParkingSystem::getPeakUsageZone() {
    int peakZoneId = -1;
    float maxUtilization = 0.0f;
    
    for (int i = 0; i < zones.getSize(); i++) {
        float util = zones[i]->getUtilization();
        if (util > maxUtilization) {
            maxUtilization = util;
            peakZoneId = zones[i]->getZoneId();
        }
    }
    
    return peakZoneId;
}

float ParkingSystem::getCancelledRatio() {
    int completed = 0;
    int cancelled = 0;
    
    for (auto it = tripHistory.begin(); it != tripHistory.end(); ++it) {
        if ((*it).completed) {
            completed++;
        } else {
            cancelled++;
        }
    }
    
    int total = completed + cancelled;
    if (total == 0) return 0.0f;
    return (float)cancelled / total;
}

int ParkingSystem::getTotalRequests() const {
    return requests.getSize();
}

int ParkingSystem::getCompletedTrips() const {
    int count = 0;
    for (auto it = tripHistory.begin(); it != tripHistory.end(); ++it) {
        if ((*it).completed) {
            count++;
        }
    }
    return count;
}

int ParkingSystem::getCancelledRequests() const {
    int count = 0;
    for (auto it = tripHistory.begin(); it != tripHistory.end(); ++it) {
        if (!(*it).completed) {
            count++;
        }
    }
    return count;
}

void ParkingSystem::displaySystemStatus() {
    std::cout << "\n===== PARKING SYSTEM STATUS =====" << std::endl;
    std::cout << "Total Zones: " << zones.getSize() << std::endl;
    std::cout << "System Utilization: " << (getSystemUtilization() * 100) << "%" << std::endl;
    std::cout << "Total Requests: " << getTotalRequests() << std::endl;
    std::cout << "Completed Trips: " << getCompletedTrips() << std::endl;
    std::cout << "Cancelled Requests: " << getCancelledRequests() << std::endl;
    std::cout << "Average Parking Duration: " << getAverageParkingDuration() << " seconds" << std::endl;
    
    int peakZone = getPeakUsageZone();
    if (peakZone != -1) {
        std::cout << "Peak Usage Zone: " << peakZone << std::endl;
    }
    std::cout << "================================\n" << std::endl;
}

void ParkingSystem::displayAllZones() {
    std::cout << "\n===== ALL ZONES =====" << std::endl;
    for (int i = 0; i < zones.getSize(); i++) {
        zones[i]->displayZoneInfo();
    }
    std::cout << "=====================\n" << std::endl;
}

void ParkingSystem::displayAllRequests() {
    std::cout << "\n===== ALL REQUESTS =====" << std::endl;
    for (int i = 0; i < requests.getSize(); i++) {
        requests[i]->displayRequestInfo();
    }
    std::cout << "========================\n" << std::endl;
}

void ParkingSystem::displayTripHistory() {
    std::cout << "\n===== TRIP HISTORY =====" << std::endl;
    int count = 0;
    for (auto it = tripHistory.begin(); it != tripHistory.end(); ++it) {
        count++;
        std::cout << count << ". Request " << (*it).requestId 
                  << ", Vehicle: " << (*it).vehicleId
                  << ", Zone: " << (*it).zoneId
                  << ", Duration: " << ((*it).endTime - (*it).startTime) << "s"
                  << ", Status: " << ((*it).completed ? "COMPLETED" : "CANCELLED")
                  << std::endl;
    }
    if (count == 0) {
        std::cout << "No trip history available" << std::endl;
    }
    std::cout << "========================\n" << std::endl;
}
