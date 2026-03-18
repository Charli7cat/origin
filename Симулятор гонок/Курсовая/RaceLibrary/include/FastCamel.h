#pragma once
#include "GroundVehicle.h"

class RACE_API FastCamel : public GroundVehicle {
public:
    FastCamel();
protected:
    double calculateRestDuration(int restCount) const override;
};