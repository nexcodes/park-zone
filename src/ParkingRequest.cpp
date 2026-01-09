#include "include/ParkingRequest.h"
#include <iostream>
#include <cstring>
#include <ctime>

ParkingRequest::ParkingRequest(int id, const char* vehicle, int zone) 
    : requestId(id), requestedZoneId(zone), allocatedSlotId(-1), 
      currentState(REQUESTED), allocationTime(0), occupiedTime(0), 
      releaseTime(0), crossZoneAllocation(false) {
    
    int len = strlen(vehicle);
    vehicleId = new char[len + 1];
    strcpy(vehicleId, vehicle);
    
    requestTime = time(nullptr);
}

ParkingRequest::~ParkingRequest() {
    delete[] vehicleId;
}

int ParkingRequest::getRequestId() const {
    return requestId;
}

const char* ParkingRequest::getVehicleId() const {
    return vehicleId;
}

int ParkingRequest::getRequestedZoneId() const {
    return requestedZoneId;
}

int ParkingRequest::getAllocatedSlotId() const {
    return allocatedSlotId;
}

RequestState ParkingRequest::getState() const {
    return currentState;
}

long long ParkingRequest::getRequestTime() const {
    return requestTime;
}

long long ParkingRequest::getAllocationTime() const {
    return allocationTime;
}

long long ParkingRequest::getOccupiedTime() const {
    return occupiedTime;
}

long long ParkingRequest::getReleaseTime() const {
    return releaseTime;
}

bool ParkingRequest::isCrossZone() const {
    return crossZoneAllocation;
}

bool ParkingRequest::setState(RequestState newState) {
    if (!StateValidator::isValidTransition(currentState, newState)) {
        std::cout << "Invalid transition: " 
                  << StateValidator::getErrorMessage(currentState, newState) 
                  << std::endl;
        return false;
    }
    currentState = newState;
    return true;
}

bool ParkingRequest::allocateSlot(int slotId, bool isCrossZone) {
    if (!setState(ALLOCATED)) {
        return false;
    }
    
    allocatedSlotId = slotId;
    crossZoneAllocation = isCrossZone;
    allocationTime = time(nullptr);
    return true;
}

bool ParkingRequest::markOccupied() {
    if (!setState(OCCUPIED)) {
        return false;
    }
    
    occupiedTime = time(nullptr);
    return true;
}

bool ParkingRequest::markReleased() {
    if (!setState(RELEASED)) {
        return false;
    }
    
    releaseTime = time(nullptr);
    return true;
}

bool ParkingRequest::cancel() {
    if (!setState(CANCELLED)) {
        return false;
    }
    return true;
}

long long ParkingRequest::getParkingDuration() const {
    if (occupiedTime > 0 && releaseTime > 0) {
        return releaseTime - occupiedTime;
    }
    return 0;
}

void ParkingRequest::displayRequestInfo() const {
    std::cout << "Request ID: " << requestId 
              << ", Vehicle: " << vehicleId
              << ", Requested Zone: " << requestedZoneId
              << ", State: " << getStateString();
    
    if (allocatedSlotId != -1) {
        std::cout << ", Slot: " << allocatedSlotId;
        if (crossZoneAllocation) {
            std::cout << " (Cross-Zone)";
        }
    }
    
    if (currentState == RELEASED) {
        std::cout << ", Duration: " << getParkingDuration() << "s";
    }
    
    std::cout << std::endl;
}

const char* ParkingRequest::getStateString() const {
    return StateValidator::getStateString(currentState);
}
