#pragma once
#include "GroundVehicle.h"

class RACE_API Centaur : public GroundVehicle {
public:
    Centaur();
protected:
    double calculateRestDuration(int restCount) const override;
};
