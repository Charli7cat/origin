#pragma once
#include "AirVehicle.h"

class RACE_API MagicCarpet : public AirVehicle {
public:
    MagicCarpet();
protected:
    double getDistanceReduction(double distance) const override;
};