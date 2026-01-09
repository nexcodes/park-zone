#include "include/AnalyticsEngine.h"
#include "include/ParkingSystem.h"
#include <iostream>
#include <iomanip>
#include <cstring>

AnalyticsEngine::AnalyticsEngine(LinkedList<TripHistory>* history, DynamicArray<Zone*>* zoneList)
    : tripHistory(history), zones(zoneList), totalRolledBackOperations(0) {}

long long AnalyticsEngine::calculateTotalDuration(bool completedOnly) {
    long long totalDuration = 0;
    for (auto it = tripHistory->begin(); it != tripHistory->end(); ++it) {
        if (!completedOnly || (*it).completed) {
            totalDuration += ((*it).endTime - (*it).startTime);
        }
    }
    return totalDuration;
}

int AnalyticsEngine::countTrips(bool completedOnly) {
    int count = 0;
    for (auto it = tripHistory->begin(); it != tripHistory->end(); ++it) {
        if (!completedOnly || (*it).completed) {
            count++;
        }
    }
    return count;
}

float AnalyticsEngine::calculateAverageParkingDuration() {
    int completedCount = countTrips(true);
    if (completedCount == 0) return 0.0f;
    
    long long totalDuration = calculateTotalDuration(true);
    return (float)totalDuration / completedCount;
}

float AnalyticsEngine::calculateZoneUtilization(int zoneId) {
    for (int i = 0; i < zones->getSize(); i++) {
        if ((*zones)[i]->getZoneId() == zoneId) {
            return (*zones)[i]->getUtilization();
        }
    }
    return 0.0f;
}

float AnalyticsEngine::calculateSystemUtilization() {
    int totalSlots = 0;
    int occupiedSlots = 0;
    
    for (int i = 0; i < zones->getSize(); i++) {
        totalSlots += (*zones)[i]->getTotalSlots();
        occupiedSlots += (*zones)[i]->getOccupiedSlots();
    }
    
    if (totalSlots == 0) return 0.0f;
    return (float)occupiedSlots / totalSlots;
}

int AnalyticsEngine::findPeakUsageZone() {
    int peakZoneId = -1;
    float maxUtilization = 0.0f;
    
    for (int i = 0; i < zones->getSize(); i++) {
        float util = (*zones)[i]->getUtilization();
        if (util > maxUtilization) {
            maxUtilization = util;
            peakZoneId = (*zones)[i]->getZoneId();
        }
    }
    
    return peakZoneId;
}

float AnalyticsEngine::calculateCancelledRatio() {
    int completed = countTrips(true);
    int total = tripHistory->getSize();
    
    if (total == 0) return 0.0f;
    return (float)(total - completed) / total;
}

void AnalyticsEngine::calculateZoneSpecificStats(Zone* zone, ZoneStats& stats) {
    stats.zoneId = zone->getZoneId();
    stats.zoneName = zone->getZoneName();
    stats.totalCapacity = zone->getTotalSlots();
    stats.currentOccupied = zone->getOccupiedSlots();
    stats.utilizationRate = zone->getUtilization();
    
    // Calculate trips for this zone
    long long totalDuration = 0;
    int tripCount = 0;
    
    for (auto it = tripHistory->begin(); it != tripHistory->end(); ++it) {
        if ((*it).zoneId == zone->getZoneId()) {
            if ((*it).completed) {
                stats.totalTripsCompleted++;
                totalDuration += ((*it).endTime - (*it).startTime);
                tripCount++;
            } else {
                stats.totalTripsCancelled++;
            }
        }
    }
    
    if (tripCount > 0) {
        stats.avgParkingDuration = (float)totalDuration / tripCount;
    }
}

SystemAnalytics AnalyticsEngine::getSystemAnalytics() {
    SystemAnalytics analytics;
    
    // Slot statistics
    analytics.totalSlots = 0;
    analytics.occupiedSlots = 0;
    for (int i = 0; i < zones->getSize(); i++) {
        analytics.totalSlots += (*zones)[i]->getTotalSlots();
        analytics.occupiedSlots += (*zones)[i]->getOccupiedSlots();
    }
    analytics.availableSlots = analytics.totalSlots - analytics.occupiedSlots;
    analytics.systemUtilization = calculateSystemUtilization();
    
    // Request statistics
    analytics.completedTrips = countTrips(true);
    analytics.cancelledRequests = tripHistory->getSize() - analytics.completedTrips;
    analytics.totalRequests = tripHistory->getSize();
    analytics.avgParkingDuration = calculateAverageParkingDuration();
    analytics.cancelledRatio = calculateCancelledRatio();
    analytics.peakZoneId = findPeakUsageZone();
    
    // Count allocation types
    analytics.crossZoneAllocations = 0;
    analytics.sameZoneAllocations = 0;
    // Note: Would need to track this in ParkingRequest to get accurate counts
    
    // Simulated revenue (base rate * completed trips)
    analytics.totalRevenue = analytics.completedTrips * 100; // $100 base rate
    
    return analytics;
}

DynamicArray<ZoneStats> AnalyticsEngine::getZoneStatistics() {
    DynamicArray<ZoneStats> stats;
    
    float maxUtil = 0.0f;
    int peakZoneId = findPeakUsageZone();
    
    for (int i = 0; i < zones->getSize(); i++) {
        ZoneStats zoneStats;
        calculateZoneSpecificStats((*zones)[i], zoneStats);
        zoneStats.isPeak = (zoneStats.zoneId == peakZoneId);
        stats.add(zoneStats);
    }
    
    return stats;
}

DynamicArray<TimeSlotStats> AnalyticsEngine::getHourlyStatistics() {
    DynamicArray<TimeSlotStats> hourlyStats;
    
    // Initialize 24 hours
    for (int h = 0; h < 24; h++) {
        TimeSlotStats stats;
        stats.hour = h;
        hourlyStats.add(stats);
    }
    
    // Aggregate data by hour
    for (auto it = tripHistory->begin(); it != tripHistory->end(); ++it) {
        time_t timestamp = (*it).startTime;
        struct tm* timeinfo = localtime(&timestamp);
        int hour = timeinfo->tm_hour;
        
        if (hour >= 0 && hour < 24) {
            hourlyStats[hour].totalRequests++;
            if ((*it).completed) {
                hourlyStats[hour].completedTrips++;
                long long duration = (*it).endTime - (*it).startTime;
                // Running average
                int n = hourlyStats[hour].completedTrips;
                hourlyStats[hour].avgDuration = 
                    (hourlyStats[hour].avgDuration * (n - 1) + duration) / n;
            } else {
                hourlyStats[hour].cancelledRequests++;
            }
        }
    }
    
    return hourlyStats;
}

float AnalyticsEngine::getTrendUtilization(int lastNHours) {
    // Simplified: return current utilization
    // In full implementation, would track historical utilization data
    return calculateSystemUtilization();
}

int AnalyticsEngine::predictPeakHour() {
    DynamicArray<TimeSlotStats> hourlyStats = getHourlyStatistics();
    
    int peakHour = 0;
    int maxRequests = 0;
    
    for (int i = 0; i < hourlyStats.getSize(); i++) {
        if (hourlyStats[i].totalRequests > maxRequests) {
            maxRequests = hourlyStats[i].totalRequests;
            peakHour = hourlyStats[i].hour;
        }
    }
    
    return peakHour;
}

void AnalyticsEngine::markOperationRolledBack(int requestId) {
    totalRolledBackOperations++;
    // In full implementation, would mark specific trip as rolled back
}

int AnalyticsEngine::getRolledBackCount() const {
    return totalRolledBackOperations;
}

long long AnalyticsEngine::calculateRevenue(float baseRate, float crossZonePenalty) {
    long long revenue = 0;
    
    for (auto it = tripHistory->begin(); it != tripHistory->end(); ++it) {
        if ((*it).completed) {
            long long duration = (*it).endTime - (*it).startTime;
            long long hours = (duration + 3599) / 3600; // Round up to nearest hour
            if (hours < 1) hours = 1;
            
            revenue += (long long)(baseRate * hours);
            // Note: Would need cross-zone flag in TripHistory to apply penalty
        }
    }
    
    return revenue;
}

void AnalyticsEngine::generateSummaryReport() {
    SystemAnalytics analytics = getSystemAnalytics();
    
    std::cout << "\n╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║          PARKING SYSTEM ANALYTICS SUMMARY                  ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝\n" << std::endl;
    
    std::cout << "SYSTEM OVERVIEW:" << std::endl;
    std::cout << "  Total Parking Slots:     " << analytics.totalSlots << std::endl;
    std::cout << "  Currently Occupied:      " << analytics.occupiedSlots << std::endl;
    std::cout << "  Available Slots:         " << analytics.availableSlots << std::endl;
    std::cout << "  System Utilization:      " << std::fixed << std::setprecision(1) 
              << (analytics.systemUtilization * 100) << "%" << std::endl;
    
    std::cout << "\nREQUEST STATISTICS:" << std::endl;
    std::cout << "  Total Requests:          " << analytics.totalRequests << std::endl;
    std::cout << "  Completed Trips:         " << analytics.completedTrips << std::endl;
    std::cout << "  Cancelled Requests:      " << analytics.cancelledRequests << std::endl;
    std::cout << "  Cancellation Rate:       " << std::fixed << std::setprecision(1) 
              << (analytics.cancelledRatio * 100) << "%" << std::endl;
    
    std::cout << "\nPERFORMANCE METRICS:" << std::endl;
    std::cout << "  Avg Parking Duration:    " << std::fixed << std::setprecision(0) 
              << analytics.avgParkingDuration << " seconds" << std::endl;
    std::cout << "  Peak Usage Zone:         " << analytics.peakZoneId << std::endl;
    std::cout << "  Rolled Back Operations:  " << totalRolledBackOperations << std::endl;
    
    std::cout << "\nREVENUE (Simulated):" << std::endl;
    std::cout << "  Estimated Revenue:       $" << analytics.totalRevenue << std::endl;
    
    std::cout << "\n════════════════════════════════════════════════════════════\n" << std::endl;
}

void AnalyticsEngine::generateDetailedReport() {
    std::cout << "\n╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║          DETAILED ANALYTICS REPORT                         ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝\n" << std::endl;
    
    generateSummaryReport();
    
    std::cout << "\nZONE-BY-ZONE ANALYSIS:" << std::endl;
    DynamicArray<ZoneStats> zoneStats = getZoneStatistics();
    
    for (int i = 0; i < zoneStats.getSize(); i++) {
        std::cout << "\n  Zone " << zoneStats[i].zoneId << " (" << zoneStats[i].zoneName << ")";
        if (zoneStats[i].isPeak) std::cout << " [PEAK]";
        std::cout << ":" << std::endl;
        std::cout << "    Capacity:           " << zoneStats[i].totalCapacity << std::endl;
        std::cout << "    Occupied:           " << zoneStats[i].currentOccupied << std::endl;
        std::cout << "    Utilization:        " << std::fixed << std::setprecision(1) 
                  << (zoneStats[i].utilizationRate * 100) << "%" << std::endl;
        std::cout << "    Completed Trips:    " << zoneStats[i].totalTripsCompleted << std::endl;
        std::cout << "    Cancelled Trips:    " << zoneStats[i].totalTripsCancelled << std::endl;
        std::cout << "    Avg Duration:       " << std::fixed << std::setprecision(0) 
                  << zoneStats[i].avgParkingDuration << "s" << std::endl;
    }
    
    std::cout << "\n════════════════════════════════════════════════════════════\n" << std::endl;
}

void AnalyticsEngine::generateZoneReport(int zoneId) {
    std::cout << "\n╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║          ZONE " << zoneId << " DETAILED REPORT                            ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝\n" << std::endl;
    
    DynamicArray<ZoneStats> allStats = getZoneStatistics();
    
    for (int i = 0; i < allStats.getSize(); i++) {
        if (allStats[i].zoneId == zoneId) {
            ZoneStats& stats = allStats[i];
            
            std::cout << "Zone Name:              " << stats.zoneName << std::endl;
            std::cout << "Total Capacity:         " << stats.totalCapacity << " slots" << std::endl;
            std::cout << "Currently Occupied:     " << stats.currentOccupied << " slots" << std::endl;
            std::cout << "Available:              " << (stats.totalCapacity - stats.currentOccupied) << " slots" << std::endl;
            std::cout << "Utilization Rate:       " << std::fixed << std::setprecision(1) 
                      << (stats.utilizationRate * 100) << "%" << std::endl;
            std::cout << "\nTrip Statistics:" << std::endl;
            std::cout << "  Completed Trips:      " << stats.totalTripsCompleted << std::endl;
            std::cout << "  Cancelled Trips:      " << stats.totalTripsCancelled << std::endl;
            std::cout << "  Avg Parking Duration: " << std::fixed << std::setprecision(0) 
                      << stats.avgParkingDuration << " seconds" << std::endl;
            
            if (stats.isPeak) {
                std::cout << "\n*** This is the PEAK USAGE ZONE ***" << std::endl;
            }
            
            std::cout << "\n════════════════════════════════════════════════════════════\n" << std::endl;
            return;
        }
    }
    
    std::cout << "Zone " << zoneId << " not found." << std::endl;
}

void AnalyticsEngine::generateTimeBasedReport() {
    std::cout << "\n╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║          HOURLY USAGE PATTERN REPORT                       ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝\n" << std::endl;
    
    DynamicArray<TimeSlotStats> hourlyStats = getHourlyStatistics();
    
    int peakHour = predictPeakHour();
    std::cout << "Predicted Peak Hour: " << peakHour << ":00" << std::endl;
    std::cout << "\nHourly Breakdown:\n" << std::endl;
    
    std::cout << std::setw(6) << "Hour" 
              << std::setw(12) << "Requests"
              << std::setw(12) << "Completed"
              << std::setw(12) << "Cancelled"
              << std::setw(15) << "Avg Duration" << std::endl;
    std::cout << std::string(57, '-') << std::endl;
    
    for (int i = 0; i < hourlyStats.getSize(); i++) {
        if (hourlyStats[i].totalRequests > 0) {
            std::cout << std::setw(4) << hourlyStats[i].hour << ":00"
                      << std::setw(12) << hourlyStats[i].totalRequests
                      << std::setw(12) << hourlyStats[i].completedTrips
                      << std::setw(12) << hourlyStats[i].cancelledRequests
                      << std::setw(12) << std::fixed << std::setprecision(0) 
                      << hourlyStats[i].avgDuration << "s";
            
            if (hourlyStats[i].hour == peakHour) {
                std::cout << "  [PEAK]";
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << "\n════════════════════════════════════════════════════════════\n" << std::endl;
}
