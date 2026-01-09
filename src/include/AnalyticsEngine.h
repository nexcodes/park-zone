#ifndef ANALYTICS_ENGINE_H
#define ANALYTICS_ENGINE_H

#include "LinkedList.h"
#include "Zone.h"
#include "DynamicArray.h"
#include <ctime>

// Forward declaration
struct TripHistory;

// Time-based statistics
struct TimeSlotStats {
    int hour;                // 0-23
    int totalRequests;
    int completedTrips;
    int cancelledRequests;
    float avgDuration;
    
    TimeSlotStats() : hour(0), totalRequests(0), completedTrips(0), 
                     cancelledRequests(0), avgDuration(0.0f) {}
};

// Zone statistics
struct ZoneStats {
    int zoneId;
    const char* zoneName;
    int totalCapacity;
    int currentOccupied;
    int totalTripsCompleted;
    int totalTripsCancelled;
    float utilizationRate;
    float avgParkingDuration;
    bool isPeak;
    
    ZoneStats() : zoneId(0), zoneName(nullptr), totalCapacity(0), 
                 currentOccupied(0), totalTripsCompleted(0), 
                 totalTripsCancelled(0), utilizationRate(0.0f), 
                 avgParkingDuration(0.0f), isPeak(false) {}
};

// System-wide analytics
struct SystemAnalytics {
    int totalSlots;
    int occupiedSlots;
    int availableSlots;
    float systemUtilization;
    int totalRequests;
    int activeRequests;
    int completedTrips;
    int cancelledRequests;
    float avgParkingDuration;
    float cancelledRatio;
    int peakZoneId;
    long long totalRevenue;  // Simulated revenue
    int crossZoneAllocations;
    int sameZoneAllocations;
};

class AnalyticsEngine {
private:
    LinkedList<TripHistory>* tripHistory;
    DynamicArray<Zone*>* zones;
    int totalRolledBackOperations;
    
    // Helper methods
    long long calculateTotalDuration(bool completedOnly);
    int countTrips(bool completedOnly);
    void calculateZoneSpecificStats(Zone* zone, ZoneStats& stats);
    
public:
    // Constructor
    AnalyticsEngine(LinkedList<TripHistory>* history, DynamicArray<Zone*>* zoneList);
    
    // Core Analytics Methods
    float calculateAverageParkingDuration();
    float calculateZoneUtilization(int zoneId);
    float calculateSystemUtilization();
    int findPeakUsageZone();
    float calculateCancelledRatio();
    
    // Enhanced Analytics
    SystemAnalytics getSystemAnalytics();
    DynamicArray<ZoneStats> getZoneStatistics();
    DynamicArray<TimeSlotStats> getHourlyStatistics();
    
    // Trend Analysis
    float getTrendUtilization(int lastNHours);
    int predictPeakHour();
    
    // Rollback-Aware Analytics
    void markOperationRolledBack(int requestId);
    int getRolledBackCount() const;
    
    // Reporting
    void generateSummaryReport();
    void generateDetailedReport();
    void generateZoneReport(int zoneId);
    void generateTimeBasedReport();
    
    // Revenue Calculation (Simulated)
    long long calculateRevenue(float baseRate, float crossZonePenalty);
};

#endif // ANALYTICS_ENGINE_H
