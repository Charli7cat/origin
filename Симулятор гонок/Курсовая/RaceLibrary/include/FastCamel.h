#pragma once
#include "GroundVehicle.h"

class FastCamel : public GroundVehicle {
public:
    FastCamel();
protected:
    double calculateRestDuration(int restCount) const override;
};