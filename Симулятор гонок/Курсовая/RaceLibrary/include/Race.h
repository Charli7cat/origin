#pragma once
#include <vector>
#include <memory>
#include <set>
#include <algorithm>
#include <string>
#include "Vehicle.h"
#include "RaceTypes.h"

struct RaceResult {
    std::string name;
    double time;
    
    RaceResult(const std::string& n, double t) : name(n), time(t) {}
    
    bool operator<(const RaceResult& other) const {
        return time < other.time;
    }
};

class Race {
private:
    double distance;
    RaceType type;
    std::vector<std::unique_ptr<Vehicle>> participants;
    std::set<std::string> registeredTypes;
    
    bool isTypeAllowed(VehicleType vehicleType) const {
        if (type == RaceType::ANY) return true;
        if (type == RaceType::GROUND_ONLY && vehicleType == VehicleType::GROUND) return true;
        if (type == RaceType::AIR_ONLY && vehicleType == VehicleType::AIR) return true;
        return false;
    }
    
public:
    Race(double dist, RaceType t) : distance(dist), type(t) {}
    
    bool registerVehicle(std::unique_ptr<Vehicle> vehicle) {
        if (!isTypeAllowed(vehicle->getType())) {
            return false;
        }
        
        if (registeredTypes.find(vehicle->getName()) != registeredTypes.end()) {
            return false;
        }
        
        registeredTypes.insert(vehicle->getName());
        participants.push_back(std::move(vehicle));
        return true;
    }
    
    bool canStart() const {
        return participants.size() >= 2;
    }
    
    std::vector<RaceResult> start() {
        std::vector<RaceResult> results;
        
        for (const auto& v : participants) {
            double time = v->calculateTime(distance);
            results.emplace_back(v->getName(), time);
        }
        
        std::sort(results.begin(), results.end());
        return results;
    }
    
    int getParticipantCount() const {
        return static_cast<int>(participants.size());
    }
};