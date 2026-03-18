#pragma once
#include <string>
#include "RaceTypes.h"
#include "Export.h"

class RACE_API Vehicle {
protected:
    std::string name;
    double speed;
    
public:
    Vehicle() : speed(0) {}
    virtual ~Vehicle() {}
    
    virtual double calculateTime(double distance) const = 0;
    virtual VehicleType getType() const = 0;
    
    std::string getName() const { return name; }
};