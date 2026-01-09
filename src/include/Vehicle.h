#ifndef VEHICLE_H
#define VEHICLE_H

class Vehicle {
private:
    char* vehicleId;
    int preferredZoneId;
    char* ownerName;
    
public:
    // Constructors & Destructor
    Vehicle(const char* id, int preferredZone);
    Vehicle(const char* id, int preferredZone, const char* owner);
    ~Vehicle();
    
    // Getters
    const char* getVehicleId() const;
    int getPreferredZoneId() const;
    const char* getOwnerName() const;
    
    // Setters
    void setPreferredZone(int zoneId);
    
    // Display
    void displayVehicleInfo() const;
};

#endif // VEHICLE_H
