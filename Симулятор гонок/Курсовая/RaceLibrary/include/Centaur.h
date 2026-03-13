#pragma once
#include "GroundVehicle.h"

class Centaur : public GroundVehicle {
public:
    Centaur();
protected:
    double calculateRestDuration(int restCount) const override;
};
