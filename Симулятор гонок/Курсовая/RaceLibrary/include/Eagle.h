#pragma once
#include "AirVehicle.h"

class RACE_API Eagle : public AirVehicle {
public:
    Eagle();
protected:
    double getDistanceReduction(double distance) const override;
};