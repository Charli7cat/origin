#pragma once
#include "GroundVehicle.h"

class RACE_API AllTerrainBoots : public GroundVehicle {
public:
    AllTerrainBoots();
protected:
    double calculateRestDuration(int restCount) const override;
};