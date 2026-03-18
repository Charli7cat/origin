#pragma once
#include "Vehicle.h"

class RACE_API AirVehicle : public Vehicle {
protected:
    virtual double getDistanceReduction(double distance) const = 0;
    
public:
    double calculateTime(double distance) const override {
        double reduction = getDistanceReduction(distance);
        double reducedDistance = distance * reduction;
        return reducedDistance / speed;
    }
    
    VehicleType getType() const override { return VehicleType::AIR; }
};