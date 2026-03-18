#include "Race.h"

Race::Race(double dist, RaceType t) : distance(dist), type(t) {}

Race::~Race() = default;

bool Race::isTypeAllowed(VehicleType vehicleType) const {
    if (type == RaceType::ANY) return true;
    if (type == RaceType::GROUND_ONLY && vehicleType == VehicleType::GROUND) return true;
    if (type == RaceType::AIR_ONLY && vehicleType == VehicleType::AIR) return true;
    return false;
}

bool Race::registerVehicle(std::unique_ptr<Vehicle> vehicle) {
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

bool Race::canStart() const {
    return participants.size() >= 2;
}

std::vector<RaceResult> Race::start() {
    std::vector<RaceResult> results;
    
    for (const auto& v : participants) {
        double time = v->calculateTime(distance);
        results.emplace_back(v->getName(), time);
    }
    
    std::sort(results.begin(), results.end());
    return results;
}

int Race::getParticipantCount() const {
    return static_cast<int>(participants.size());
}

std::vector<std::string> Race::getParticipantNames() const {
    std::vector<std::string> names;
    for (const auto& v : participants) {
        names.push_back(v->getName());
    }
    return names;
}