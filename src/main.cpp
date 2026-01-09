#include <iostream>
#include <thread>
#include <chrono>
#include "include/ParkingSystem.h"

using namespace std;

void printSeparator() {
    cout << "\n========================================\n" << endl;
}

void testBasicAllocation(ParkingSystem& system) {
    cout << "TEST 1: Basic Same-Zone Allocation" << endl;
    printSeparator();
    
    int req1 = system.createRequest("CAR001", 1);
    system.processRequest(req1);
    system.markVehicleArrived(req1);
    
    system.displayAllRequests();
    system.displayAllZones();
}

void testCrossZoneAllocation(ParkingSystem& system) {
    cout << "TEST 2: Cross-Zone Allocation" << endl;
    printSeparator();
    
    // Fill zone 1 completely
    for (int i = 0; i < 10; i++) {
        char vehicleId[20];
        sprintf(vehicleId, "FILL%02d", i);
        int req = system.createRequest(vehicleId, 1);
        system.processRequest(req);
        system.markVehicleArrived(req);
    }
    
    // Next request should go to adjacent zone 2
    int reqCross = system.createRequest("CROSS001", 1);
    system.processRequest(reqCross);
    
    system.displayAllRequests();
    system.displayAllZones();
}

void testCancellation(ParkingSystem& system) {
    cout << "TEST 3: Request Cancellation" << endl;
    printSeparator();
    
    // Cancel before allocation
    int req1 = system.createRequest("CANCEL1", 1);
    cout << "\nCancelling before allocation:" << endl;
    system.cancelRequest(req1);
    
    // Cancel after allocation
    int req2 = system.createRequest("CANCEL2", 1);
    system.processRequest(req2);
    cout << "\nCancelling after allocation:" << endl;
    system.cancelRequest(req2);
    
    system.displayAllRequests();
}

void testInvalidStateTransitions(ParkingSystem& system) {
    cout << "TEST 4: Invalid State Transitions" << endl;
    printSeparator();
    
    int req = system.createRequest("INVALID01", 1);
    
    // Try to mark as arrived without allocation
    cout << "Trying to mark as arrived without allocation:" << endl;
    system.markVehicleArrived(req);
    
    // Allocate properly
    system.processRequest(req);
    
    // Try to exit without arriving
    cout << "\nTrying to exit without marking as arrived:" << endl;
    system.markVehicleExited(req);
    
    system.displayAllRequests();
}

void testRollback(ParkingSystem& system) {
    cout << "TEST 5: Rollback Operations" << endl;
    printSeparator();
    
    // Create and process 3 requests
    int req1 = system.createRequest("ROLL001", 1);
    int req2 = system.createRequest("ROLL002", 1);
    int req3 = system.createRequest("ROLL003", 1);
    
    system.processRequest(req1);
    system.processRequest(req2);
    system.processRequest(req3);
    
    cout << "\nBefore rollback:" << endl;
    system.displayAllRequests();
    system.displayAllZones();
    
    // Rollback last 2 operations
    cout << "\nRolling back 2 operations..." << endl;
    system.rollbackOperations(2);
    
    cout << "\nAfter rollback:" << endl;
    system.displayAllRequests();
    system.displayAllZones();
}

void testCompleteParkingCycle(ParkingSystem& system) {
    cout << "TEST 6: Complete Parking Cycle" << endl;
    printSeparator();
    
    int req = system.createRequest("CYCLE001", 2);
    
    cout << "Step 1: Request created" << endl;
    system.displayAllRequests();
    
    system.processRequest(req);
    cout << "\nStep 2: Slot allocated" << endl;
    system.displayAllRequests();
    
    system.markVehicleArrived(req);
    cout << "\nStep 3: Vehicle arrived (parked)" << endl;
    system.displayAllRequests();
    
    // Simulate parking duration
    this_thread::sleep_for(chrono::seconds(2));
    
    system.markVehicleExited(req);
    cout << "\nStep 4: Vehicle exited" << endl;
    system.displayAllRequests();
    
    system.displayTripHistory();
}

void testAnalytics(ParkingSystem& system) {
    cout << "TEST 7: Analytics" << endl;
    printSeparator();
    
    // Create several completed trips
    for (int i = 0; i < 5; i++) {
        char vehicleId[20];
        sprintf(vehicleId, "ANALYTICS%02d", i);
        int req = system.createRequest(vehicleId, (i % 2) + 1);
        system.processRequest(req);
        system.markVehicleArrived(req);
        this_thread::sleep_for(chrono::seconds(1));
        system.markVehicleExited(req);
    }
    
    // Cancel one request
    int cancelReq = system.createRequest("CANCEL_ANALYTICS", 1);
    system.processRequest(cancelReq);
    system.cancelRequest(cancelReq);
    
    system.displaySystemStatus();
    system.displayTripHistory();
}

void testZoneUtilization(ParkingSystem& system) {
    cout << "TEST 8: Zone Utilization" << endl;
    printSeparator();
    
    // Partially fill zone 1
    for (int i = 0; i < 5; i++) {
        char vehicleId[20];
        sprintf(vehicleId, "UTIL1_%02d", i);
        int req = system.createRequest(vehicleId, 1);
        system.processRequest(req);
        system.markVehicleArrived(req);
    }
    
    // Partially fill zone 2
    for (int i = 0; i < 3; i++) {
        char vehicleId[20];
        sprintf(vehicleId, "UTIL2_%02d", i);
        int req = system.createRequest(vehicleId, 2);
        system.processRequest(req);
        system.markVehicleArrived(req);
    }
    
    cout << "Zone 1 Utilization: " << (system.getZoneUtilization(1) * 100) << "%" << endl;
    cout << "Zone 2 Utilization: " << (system.getZoneUtilization(2) * 100) << "%" << endl;
    cout << "System Utilization: " << (system.getSystemUtilization() * 100) << "%" << endl;
    cout << "Peak Usage Zone: " << system.getPeakUsageZone() << endl;
}

void testMultipleZones(ParkingSystem& system) {
    cout << "TEST 9: Multiple Zones Operation" << endl;
    printSeparator();
    
    // Distribute vehicles across zones
    for (int zone = 1; zone <= 3; zone++) {
        for (int i = 0; i < 2; i++) {
            char vehicleId[20];
            sprintf(vehicleId, "Z%d_CAR%02d", zone, i);
            int req = system.createRequest(vehicleId, zone);
            system.processRequest(req);
            system.markVehicleArrived(req);
        }
    }
    
    system.displayAllZones();
    system.displaySystemStatus();
}

void testEdgeCases(ParkingSystem& system) {
    cout << "TEST 10: Edge Cases" << endl;
    printSeparator();
    
    // Test with invalid request ID
    cout << "Test 1: Invalid request ID" << endl;
    system.processRequest(9999);
    
    // Test double allocation attempt
    cout << "\nTest 2: Double allocation attempt" << endl;
    int req = system.createRequest("EDGE001", 1);
    system.processRequest(req);
    system.processRequest(req);
    
    // Test cancel on occupied slot
    cout << "\nTest 3: Cancel occupied slot" << endl;
    int req2 = system.createRequest("EDGE002", 1);
    system.processRequest(req2);
    system.markVehicleArrived(req2);
    system.cancelRequest(req2);
    
    system.displayAllRequests();
}

int main() {
    cout << "\n**************************************************" << endl;
    cout << "*  SMART PARKING MANAGEMENT SYSTEM - TEST SUITE  *" << endl;
    cout << "**************************************************\n" << endl;
    
    // Initialize parking system
    ParkingSystem parkingSystem;
    
    // Create Zone 1: Downtown
    Zone* zone1 = new Zone(1, "Downtown");
    zone1->addParkingArea(new ParkingArea(101, 1, "Level 1", 5));
    zone1->addParkingArea(new ParkingArea(102, 1, "Level 2", 5));
    parkingSystem.addZone(zone1);
    
    // Create Zone 2: Airport
    Zone* zone2 = new Zone(2, "Airport");
    zone2->addParkingArea(new ParkingArea(201, 2, "Terminal A", 8));
    zone2->addParkingArea(new ParkingArea(202, 2, "Terminal B", 7));
    parkingSystem.addZone(zone2);
    
    // Create Zone 3: Mall
    Zone* zone3 = new Zone(3, "Mall");
    zone3->addParkingArea(new ParkingArea(301, 3, "Ground Floor", 6));
    zone3->addParkingArea(new ParkingArea(302, 3, "Basement", 4));
    parkingSystem.addZone(zone3);
    
    // Set up zone adjacencies
    zone1->addAdjacentZone(2);
    zone2->addAdjacentZone(1);
    zone2->addAdjacentZone(3);
    zone3->addAdjacentZone(2);
    
    cout << "System initialized with 3 zones:" << endl;
    cout << "- Zone 1 (Downtown): 10 slots" << endl;
    cout << "- Zone 2 (Airport): 15 slots" << endl;
    cout << "- Zone 3 (Mall): 10 slots" << endl;
    cout << "Total: 35 parking slots\n" << endl;
    
    // Run all tests
    testBasicAllocation(parkingSystem);
    
    // Create fresh system for each major test to avoid state conflicts
    ParkingSystem system2;
    Zone* z1 = new Zone(1, "Zone1");
    z1->addParkingArea(new ParkingArea(101, 1, "Area1", 3));
    z1->addAdjacentZone(2);
    Zone* z2 = new Zone(2, "Zone2");
    z2->addParkingArea(new ParkingArea(201, 2, "Area2", 5));
    system2.addZone(z1);
    system2.addZone(z2);
    testCrossZoneAllocation(system2);
    
    ParkingSystem system3;
    Zone* z3 = new Zone(1, "TestZone");
    z3->addParkingArea(new ParkingArea(101, 1, "TestArea", 5));
    system3.addZone(z3);
    testCancellation(system3);
    
    testInvalidStateTransitions(system3);
    testRollback(system3);
    testCompleteParkingCycle(parkingSystem);
    testAnalytics(parkingSystem);
    testZoneUtilization(parkingSystem);
    testMultipleZones(parkingSystem);
    testEdgeCases(parkingSystem);
    
    // Final system status
    printSeparator();
    cout << "FINAL SYSTEM STATUS:" << endl;
    parkingSystem.displaySystemStatus();
    
    cout << "\n**************************************************" << endl;
    cout << "*           ALL TESTS COMPLETED                   *" << endl;
    cout << "**************************************************\n" << endl;
    
    return 0;
}
