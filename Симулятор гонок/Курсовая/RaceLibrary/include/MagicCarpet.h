#pragma once
#include "AirVehicle.h"

class MagicCarpet : public AirVehicle {
public:
    MagicCarpet();
protected:
    double getDistanceReduction(double distance) const override;
};