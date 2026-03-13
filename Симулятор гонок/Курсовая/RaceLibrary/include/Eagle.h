#pragma once
#include "AirVehicle.h"

class Eagle : public AirVehicle {
public:
    Eagle();
protected:
    double getDistanceReduction(double distance) const override;
};