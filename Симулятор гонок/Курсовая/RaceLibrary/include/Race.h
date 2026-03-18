#pragma once
#include <vector>
#include <memory>
#include <set>
#include <algorithm>
#include <string>
#include "Vehicle.h"
#include "RaceTypes.h"
#include "Export.h"

struct RACE_API RaceResult {
    std::string name;
    double time;
    
    RaceResult(const std::string& n, double t) : name(n), time(t) {}
    
    bool operator<(const RaceResult& other) const {
        return time < other.time;
    }
};

class RACE_API Race {
private:
    double distance;
    RaceType type;
    std::vector<std::unique_ptr<Vehicle>> participants;
    std::set<std::string> registeredTypes;
    
    bool isTypeAllowed(VehicleType vehicleType) const;
    
public:
    Race(double dist, RaceType t);
    ~Race();
    
    Race(const Race&) = delete;
    Race& operator=(const Race&) = delete;
    
    Race(Race&&) = default;
    Race& operator=(Race&&) = default;
    
    bool registerVehicle(std::unique_ptr<Vehicle> vehicle);
    bool canStart() const;
    std::vector<RaceResult> start();
    
    int getParticipantCount() const;
    std::vector<std::string> getParticipantNames() const;
};