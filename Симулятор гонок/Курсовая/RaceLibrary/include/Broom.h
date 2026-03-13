#pragma once
#include "AirVehicle.h"

class Broom : public AirVehicle {
public:
    Broom();
protected:
    double getDistanceReduction(double distance) const override;
};