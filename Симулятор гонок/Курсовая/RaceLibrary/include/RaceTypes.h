#pragma once
#include <string>

enum class VehicleType {
    GROUND,
    AIR
};

enum class RaceType {
    GROUND_ONLY,
    AIR_ONLY,
    ANY
};

inline std::string raceTypeToString(RaceType type) {
    switch(type) {
        case RaceType::GROUND_ONLY: return "ground only";
        case RaceType::AIR_ONLY: return "air only";
        case RaceType::ANY: return "any type";
        default: return "unknown";
    }
}