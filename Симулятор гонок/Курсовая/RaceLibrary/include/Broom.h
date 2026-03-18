#pragma once
#include "AirVehicle.h"

class RACE_API Broom : public AirVehicle {
public:
    Broom();
protected:
    double getDistanceReduction(double distance) const override;
};