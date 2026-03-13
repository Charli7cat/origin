#pragma once
#include "GroundVehicle.h"

class Camel : public GroundVehicle {
public:
    Camel();
protected:
    double calculateRestDuration(int restCount) const override;
};