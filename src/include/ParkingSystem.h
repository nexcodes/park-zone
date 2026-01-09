#ifndef PARKING_SYSTEM_H
#define PARKING_SYSTEM_H

#include "Zone.h"
#include "Vehicle.h"
#include "ParkingRequest.h"
#include "AllocationEngine.h"
#include "RollbackManager.h"
#include "LinkedList.h"
#include "Queue.h"
#include "DynamicArray.h"

// Trip history entry
struct TripHistory {
    int requestId;
    char* vehicleId;
    int zoneId;
    long long startTime;
    long long endTime;
    bool completed;  // false if cancelled
    
    TripHistory() : requestId(0), vehicleId(nullptr), zoneId(0), 
                   startTime(0), endTime(0), completed(false) {}
    
    ~TripHistory() {
        if (vehicleId != nullptr) {
            delete[] vehicleId;
        }
    }
    
    TripHistory(const TripHistory& other) {
        requestId = other.requestId;
        zoneId = other.zoneId;
        startTime = other.startTime;
        endTime = other.endTime;
        completed = other.completed;
        
        if (other.vehicleId != nullptr) {
            int len = strlen(other.vehicleId);
            vehicleId = new char[len + 1];
            strcpy(vehicleId, other.vehicleId);
        } else {
            vehicleId = nullptr;
        }
    }
    
    TripHistory& operator=(const TripHistory& other) {
        if (this != &other) {
            if (vehicleId != nullptr) {
                delete[] vehicleId;
            }
            
            requestId = other.requestId;
            zoneId = other.zoneId;
            startTime = other.startTime;
            endTime = other.endTime;
            completed = other.completed;
            
            if (other.vehicleId != nullptr) {
                int len = strlen(other.vehicleId);
                vehicleId = new char[len + 1];
                strcpy(vehicleId, other.vehicleId);
            } else {
                vehicleId = nullptr;
            }
        }
        return *this;
    }
};

class ParkingSystem {
private:
    DynamicArray<Zone*> zones;
    DynamicArray<ParkingRequest*> requests;
    LinkedList<TripHistory> tripHistory;
    
    AllocationEngine* allocationEngine;
    RollbackManager* rollbackManager;
    
    int nextRequestId;
    
public:
    // Constructor & Destructor
    ParkingSystem();
    ~ParkingSystem();
    
    // Zone Management
    void addZone(Zone* zone);
    Zone* getZoneById(int zoneId);
    int getZoneCount() const;
    
    // Request Management
    int createRequest(const char* vehicleId, int zoneId);
    bool processRequest(int requestId);
    bool cancelRequest(int requestId);
    ParkingRequest* getRequestById(int requestId);
    
    // Slot Operations
    bool markVehicleArrived(int requestId);
    bool markVehicleExited(int requestId);
    
    // Rollback
    bool rollbackOperations(int k);
    
    // Utility - needed by other classes
    ParkingSlot* findSlotById(int slotId);
    ParkingRequest* findRequestByVehicle(const char* vehicleId);
    
    // Analytics
    float getAverageParkingDuration();
    float getZoneUtilization(int zoneId);
    float getSystemUtilization();
    int getPeakUsageZone();
    float getCancelledRatio();
    int getTotalRequests() const;
    int getCompletedTrips() const;
    int getCancelledRequests() const;
    
    // Display
    void displaySystemStatus();
    void displayAllZones();
    void displayAllRequests();
    void displayTripHistory();
};

#endif // PARKING_SYSTEM_H
