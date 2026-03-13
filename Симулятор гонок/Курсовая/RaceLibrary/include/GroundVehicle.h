#pragma once
#include "Vehicle.h"
#include <cmath>

class GroundVehicle : public Vehicle {
protected:
    double restInterval;
    virtual double calculateRestDuration(int restCount) const = 0;
    
public:
    GroundVehicle() : restInterval(0) {}
    
    double calculateTime(double distance) const override {
        double travelTime = distance / speed;
        int restStops = static_cast<int>(travelTime / restInterval);
        
        double remainder = travelTime - (restStops * restInterval);
        if (remainder < 0.0001 && restStops > 0) {
            restStops--;
        }
        
        double totalRestTime = 0;
        for (int i = 1; i <= restStops; i++) {
            totalRestTime += calculateRestDuration(i);
        }
        
        return travelTime + totalRestTime;
    }
    
    VehicleType getType() const override { return VehicleType::GROUND; }
};