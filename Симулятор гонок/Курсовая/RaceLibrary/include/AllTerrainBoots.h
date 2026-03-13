#pragma once
#include "GroundVehicle.h"

class AllTerrainBoots : public GroundVehicle {
public:
    AllTerrainBoots();
protected:
    double calculateRestDuration(int restCount) const override;
};